# Máy tính GCD Lab 05

## Mục tiêu

- Làm quen với SystemC.
- Thiết kế một mô hình SystemC tính toán ước chung lớn nhất (GCD) của hai số bằng SystemC ở cấp độ hành vi.
- Mô phỏng và kiểm tra các thành phần.

## Giới thiệu

Thuật toán được sử dụng để tính GCD như sau: Hai số được so sánh (x = y?). Nếu bằng nhau, GCD được tìm thấy. Nếu x > y, thì x = x - y. Hai số sau đó được so sánh lại. Nếu y > x, thì y = y - x. Hai số sau đó được so sánh lại. Dưới đây là một ví dụ về thuật toán:

- x = 10, y = 2
- x = y? Không, x > y, do đó x = x - y
- Trong trường hợp này, x = 10 - 2 = 8.
- x = y? Không, x > y, do đó x = x - y
- Trong trường hợp này, x = 8 - 2 = 6.
- x = y? Không, x > y, do đó x = x - y
- Trong trường hợp này, x = 6 - 2 = 4.
- x = y? Không, x > y, do đó x = x - y
- Trong trường hợp này, x = 4 - 2 = 2.
- x = y? Có, do đó GCD của 10 và 2 là 2.

**Lưu ý**: 0 không phải là đầu vào hợp lệ.

## Vấn đề thiết kế

1. Triển khai một máy trạng thái hữu hạn (FSM) trong SystemC để tính ước chung lớn nhất (GCD) của 2 số.

Thiết kế máy tính GCD nên được chia thành 2 phần - bộ điều khiển và đường dữ liệu. Bộ điều khiển là một FSM, đưa ra các lệnh cho đường dữ liệu dựa trên trạng thái hiện tại và các đầu vào bên ngoài. Đây có thể là mô tả hành vi. Đường dữ liệu chứa một danh sách các đơn vị chức năng như bộ chọn đa năng (multiplexor), thanh ghi (register), bộ trừ (subtractor) và bộ so sánh (comparator), do đó thiết kế này là cấu trúc. Bộ điều khiển cơ bản thực hiện các bước của thuật toán GCD như đã nêu ở trên. Nếu x = y, quá trình tính GCD đã hoàn tất, và chúng ta chuyển đến trạng thái cuối cùng và kích hoạt đường dữ liệu đầu ra. Đường dữ liệu thực hiện tính toán GCD thực tế. Nó bao gồm các thành phần sau:

- **Bộ chọn đa năng (Mux):** Nhận 2 đầu vào 4-bit và một đường chọn. Dựa trên đường chọn, nó xuất ra số 4-bit thứ nhất hoặc thứ hai.
- **Thanh ghi (Register):** Nhận đầu vào 4-bit, tín hiệu tải (load), tín hiệu đặt lại (reset) và tín hiệu đồng hồ (clock). Nếu tín hiệu tải ở mức cao và đồng hồ được xung, nó xuất ra số 4-bit.
- **Bộ so sánh (Comparator):** Nhận 2 số 4-bit, và kích hoạt một trong 3 tín hiệu tùy thuộc vào việc số thứ nhất nhỏ hơn, lớn hơn hoặc bằng số thứ hai.
- **Bộ trừ (Subtractor):** Nhận 2 số 4-bit, trừ số nhỏ hơn khỏi số lớn hơn.
- **Thanh ghi đầu ra (Output Register):** Lưu giữ giá trị GCD. Khi x = y, GCD đã được tìm thấy và có thể được xuất ra. Vì đây là một thực thể thanh ghi, nó cũng nên nhận tín hiệu đồng hồ và đặt lại.

## Cấu trúc mẫu của Bộ điều khiển và Đường dữ liệu

![Hình ảnh GCD Calculator](https://www.cs.ucr.edu/~vahid/sproj/SystemCLab/GCD_calc.gif)