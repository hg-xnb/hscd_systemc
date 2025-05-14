/* DHDEMO - demonstration program for Diffie-Hellman extensions to RSAREF
   Copyright (C) 1993 RSA Laboratories, a division of RSA Data
   Security, Inc. All rights reserved.
*/

#include "systemc.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "global.h"
#include "rsaref.h"
#include "nn.h"
#include "digit.h"
#include "des.h"
#include "r_random.h"
#include "rsa.h"
#include "md5.h"
#include "prime.h"
#include "extras.cpp"

SC_MODULE (dhdemo)
{

	SC_CTOR (dhdemo) 
	{
		SC_THREAD (dh_main);	
	}

	void dh_main()
	{
	  R_RANDOM_STRUCT randomStruct;
	  R_RANDOM_STRUCT randomStruct2;
	  char command[80];
	  int done = 0;
	  int num_keyexchange;
  
	  num_keyexchange = 3; 
	  Key_length = 7;

	  /* We first generate parameters, and then do some key exchange each followed by a key computation...*/
	  keys_exchanged = 0;

	  InitRandomStruct (&randomStruct);
	  DoGenerateParams (&randomStruct);      

	  if (PARAMS2_READY)
		init_gens (PARAMS1.primeLen, PARAMS2.primeLen);
	  else
		PrintError ("PARAMS2 not initilized\n", 0);
  
	  DoSetupAgreement (&randomStruct, 0);
  
	  while (keys_exchanged != num_keyexchange) 
		{
		  fprintf (stderr, "Generating key set %d \n", keys_exchanged);
		  keys_exchanged++;
		  InitRandomStruct (&randomStruct2);
		  DoGenerateParams (&randomStruct2);
		  DoSetupAgreement (&randomStruct2, 1);
		  DoComputeAgreedKey ();  

		  //Output Data
		  cout << "prime: " << (long int)(PARAMS2.prime) << endl;
		  cout << "generator: " << (long int)(PARAMS2.generator) << endl;
		}
  
	  R_RandomFinal (&randomStruct);
	    
	  sc_stop();
	 
	}

	//Begin ---- Functions from DHDEMO.C -------------------------
	void InitRandomStruct (R_RANDOM_STRUCT *randomStruct)
	{
	  static unsigned char seedByte = 0;
	  unsigned int bytesNeeded;
  
	  R_RandomInit (randomStruct);
  
	  /* Initialize with all zero seed bytes, which will not yield an actual
		   random number output.
	   */
	  while (1) {
		R_GetRandomBytesNeeded (&bytesNeeded, randomStruct);
		if (bytesNeeded == 0)
		  break;
    
		R_RandomUpdate (randomStruct, &seedByte, 1);
	  }
	}

	static int SetOptions (int argc,char *argv[])
	{
	  int i, status = 0;
  
	  for (i = 1; i < argc; i++) {
		if (argv[i][0] != '-') {
		  status = 1;
		  break;
		}
    
		if (argv[i][1] == 's')
		  SILENT_PROMPT = 1;
		else {
		  status = 1;
		  break;
		}
	  }

	  if (status)
		puts ("Usage: dhdemo [-s]\n\
	  -s silent prompts");

	  return (status);
	}

	/* Initialize the random structure with all zero seed bytes for test purposes.
	   NOTE that this will cause the output of the "random" process to be
		 the same every time.  To produce random bytes, the random struct
		 needs random seeds!
	 */

	/*static*/ void DoSetupAgreement (R_RANDOM_STRUCT *randomStruct,int priv)
	{
	  R_DH_PARAMS *params;
	  char command[80];
	  int status;
	  unsigned char *privateValue, *publicValue;
	  unsigned int privateValueLen;

	  *(&params) = &PARAMS1;

	  privateValueLen = Key_length;
	  privateValue = (unsigned char *)malloc (privateValueLen);
	  publicValue = (unsigned char *)malloc (params->primeLen);

	  /* Set up a break point with a do {} while (0) so that we can
		 zeroize the sensitive buffers before exiting.
	  */
	  do {
		if (status = R_SetupDHAgreement
			(publicValue, privateValue, privateValueLen, params, randomStruct)) {
		  PrintError ("setting up key agreement", status);
		  break;
		}
    
		if (priv)
		  {
			if (gen_privateValue != NULL)
			{
				for(int i=0;i<privateValueLen;i++)
					gen_privateValue[i] = privateValue[i];
			}
			//strncpy (gen_privateValue, privateValue, privateValueLen);
			memcpy (gen_publicValue1, publicValue, params->primeLen);
		  }
		else
		  {
		if (gen_otherPublicValue != NULL)
		  free (gen_otherPublicValue);
		gen_otherPublicValue = (unsigned char *) malloc (params->primeLen);
		memcpy (gen_otherPublicValue, publicValue, params->primeLen);
		  }
	  } while (0);

	  memset ((POINTER)privateValue, 0, privateValueLen);
	  free (privateValue);
	  free (publicValue);
	}

	/*static*/ void DoComputeAgreedKey ()
	{
	  R_DH_PARAMS *params;
	  int status;
	  unsigned char *agreedKey, *otherPublicValue, *privateValue;
	  unsigned int otherPublicValueLen, privateValueLen;

	  if ((keys_exchanged % 2) == 0)
		*(&params) = &PARAMS1;
	  else
		*(&params) = &PARAMS2;

	  otherPublicValue = gen_otherPublicValue;
	  privateValue = gen_privateValue;
	  agreedKey = (unsigned char *)malloc (params->primeLen);

	  /* Set up a break point with a do {} while (0) so that we can
		   zeroize the sensitive buffers before exiting.
	   */
	  do {
		privateValueLen = Key_length;
		otherPublicValueLen = Key_length;
		if (status = R_ComputeDHAgreedKey
			(agreedKey, otherPublicValue, privateValue, privateValueLen, params)) {
		  PrintError ("computing agreed-upon key", status);
		  break;
		}
	  } while (0);
  
	  memset ((POINTER)privateValue, 0, privateValueLen);
	  memset ((POINTER)agreedKey, 0, params->primeLen);
	  free (agreedKey);
	}

	/*static*/ void DoGenerateParams (R_RANDOM_STRUCT *randomStruct)
	{
	  char command[80];
	  int status, primeBits, subPrimeBits;

	  primeBits = Key_length;
	  subPrimeBits = Key_length / 2;
  
	  if (PARAMS2_READY) {
		free (PARAMS2.prime);
		free (PARAMS2.generator);
	  }
	  PARAMS2.prime = (unsigned char *)malloc (DH_PRIME_LEN (primeBits));
	  PARAMS2.generator = (unsigned char *)malloc (DH_PRIME_LEN (primeBits));
  
	  if (status = R_GenerateDHParams
		  (&PARAMS2, primeBits, subPrimeBits, randomStruct)) {
		PrintError ("generating parameters", status);
		return;
	  }

	  PrintMessage ("Parameters 2 are now ready to use.");
	  PARAMS2_READY = 1;
	}

	void init_gens(int size1,int size2)
	{
	  gen_privateValue = (unsigned char *) malloc (Key_length);
	  gen_publicValue1 = (unsigned char *) malloc (size1);
	  gen_publicValue2 = (unsigned char *) malloc (size2);
	}

	/* Set options from command line and return 0 for success, 1 for bad format.
	 */


	static void WriteParams2 ()
	{
	  FILE *file;
	  char filename[256];
  
	  while (1) {
		GetCommand
		  (filename, sizeof (filename),
		   "Enter filename to save the parameters (blank to not save): ");
		if (! *filename)
		  return;
    
		if ((file = fopen (filename, "w")) != NULL)
		  break;
    
		PrintError ("ERROR: Cannot open a file prime.key. Check permissions....", 0);
	  }
  
	  fprintf (file, "Parameters:\n");
	  fprintf (file, "  prime: ");
	  WriteBigInteger (file, PARAMS2.prime, PARAMS2.primeLen);
	  fprintf (file, "  generator: ");
	  WriteBigInteger (file, PARAMS2.generator, PARAMS2.generatorLen);
  
	  if (file != stdout)
		fclose (file);
	}

	/* Write the byte string 'integer' to 'file', skipping over leading zeros.
	 */
	static void WriteBigInteger (FILE *file,unsigned char *integer,unsigned int integerLen)
	{
	  while (*integer == 0 && integerLen > 0) {
		integer++;
		integerLen--;
	  }
  
	  if (integerLen == 0) {
		/* Special case, just print a zero. */
		fprintf (file, "00\n");
		return;
	  }
  
	  for (; integerLen > 0; integerLen--)
		fprintf (file, "%02x ", (unsigned int)(*integer++));

	  fprintf (file, "\n");
	}

	/* Use the prompt to ask the user to use parameters 1 or 2 and
		 point params to the answer.
	   Return 0 on success or 1 if user cancels by entering a blank.
	 */
	static int GetParams (R_DH_PARAMS **params,char *prompt)
	{
	  char command[80];
  
	  while (1) {
		GetCommand (command, sizeof (command), prompt);

		switch (*command) {
		case '\0':
		  return (1);
      
		case '1':
		  *params = &PARAMS1;
		  return (0);
      
		case '2':
		  if (!PARAMS2_READY) {
			PrintError
			  ("ERROR: Parameters 2 have not been generated yet.  Try Again.", 0);
			break;
		  }
		  else {
			*params = &PARAMS2;
			return (0);
		  }
      
		default:
		  if (PARAMS2_READY)
			PrintError ("ERROR: Please enter 1 or 2.  Try again.", 0);
		  else
			PrintError ("ERROR: Please enter 1.  Try again.", 0);
		  break;
		}
	  }
	}

	/* Read a file of up to length maxBlockLen bytes, storing it in
		 block and returning its length in blockLen.
	   Ask for the filename using the given prompt string.
	   Return 0 on success or 1 if error or if user cancels by entering a blank.
	 */
	static int ReadBlock (unsigned char *block,unsigned int *blockLen,unsigned int maxBlockLen,char *prompt) 
	{
	  FILE *file;
	  int status;
	  char filename[256];
	  unsigned char dummy;
  
	  /* fread () returns the number of items read in.  Expect an end of file
		   after the read.
	   */
	  *blockLen = fread (block, 1, maxBlockLen, file);
	  if (*blockLen == maxBlockLen)
		/* Read exactly maxBlockLen bytes, so reading one more will set 
			 end of file if there were exactly maxBlockLen bytes in the file.
		 */
		fread (&dummy, 1, 1, file);
  
	  if (!feof (file)) {
		PrintError ("ERROR: Cannot read file or file is too large.", 0);
		status = 1;
	  }
	  else
		status = 0;
  
	  fclose (file);
	  return (status);
	}

	/* Write block oflength blockLen to a file.
	   Ask for the filename using the given prompt string.
	   Return 0 on success or 1 if error or if user cancels by entering a blank.
	 */
	static int WriteBlock (unsigned char *block,unsigned int blockLen,char *prompt,char filename[256]) 
	{
	  FILE *file;
	  int status;
  
	  if ((file = fopen (filename, "wb")) != NULL)
		PrintError ("ERROR: Cannot open a file with that name.  Try again.", 0);
  
	  status = 0;
	  if (fwrite (block, 1, blockLen, file) < blockLen) {
		PrintError ("ERROR: Cannot write file.", 0);
		status = 1;
	  }
	  else {
		if (file == stdout)
		  /* Printing to screen, so print a new line. */
		  printf ("\n");
	  }
  
	  if (file != stdout)
		fclose (file);
	  return (status);
	}


	static void PrintMessage (const char *message)
	{
	  if (!SILENT_PROMPT) {
		puts (message);
		fflush (stdout);
	  }
	}

	/* If type is zero, simply print the task string, otherwise convert the
		 type to a string and print task and type.
	 */
	static void PrintError (const char *task,int type)
	{
	  char *typeString, buf[80];

	  if (type == 0) {
		puts (task);
		return;
	  }
  
	  /* Convert the type to a string if it is recognized.
	   */
	  switch (type) {
	  case RE_CONTENT_ENCODING:
		typeString = "(Encrypted) content has RFC 1113 encoding error";
		break;
	  case RE_DIGEST_ALGORITHM:
		typeString = "Message-digest algorithm is invalid";
		break;
	  case RE_KEY:
		typeString = "Recovered DES key cannot decrypt encrypted content or encrypt signature";
		break;
	  case RE_KEY_ENCODING:
		typeString = "Encrypted key has RFC 1113 encoding error";
		break;
	  case RE_MODULUS_LEN:
		typeString = "Modulus length is invalid";
		break;
	  case RE_NEED_RANDOM:
		typeString = "Random structure is not seeded";
		break;
	  case RE_PRIVATE_KEY:
		typeString = "Private key cannot encrypt message digest, or cannot decrypt encrypted key";
		break;
	  case RE_PUBLIC_KEY:
		typeString = "Public key cannot encrypt DES key, or cannot decrypt signature";
		break;
	  case RE_SIGNATURE:
		typeString = "Signature on content or block is incorrect";
		break;
	  case RE_SIGNATURE_ENCODING:
		typeString = "(Encrypted) signature has RFC 1113 encoding error";
		break;
    
	  default:
		sprintf (buf, "Code 0x%04x", type);
		typeString = buf;
	  }

	  printf ("ERROR: %s while %s\n", typeString, task);  
	  fflush (stdout);
	}

	static void GetCommand (char *command,unsigned int maxCommandSize,char *prompt)
	{
	  unsigned int i;
  
	  if (!SILENT_PROMPT) {
		printf ("%s\n", prompt);  
		fflush (stdout);
	  }

	  fgets (command, maxCommandSize, stdin);
  
	  /* Replace the line terminator with a '\0'.
	   */
	  for (i = 0; command[i] != '\0'; i++) {
		if (command[i] == '\012' || command[i] == '\015' ||
			i == (maxCommandSize - 1)) {
		  command[i] = '\0';
		  return;
		}
	  }
	}
	//End ---- Functions from DHDEMO.C ----------------------------

	
	

};

int sc_main(int argc, char* argv[])
{

	dhdemo	DH("DH");

	sc_start(2000000,SC_NS);

	return(0);
}