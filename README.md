# Dự án trang trí cây thông Noel cho làng trẻ em SOS Việt Nam

## 1. Nội dung yêu cầu từ SOS Việt Nam:

**1.1 Yêu cầu về ánh sáng sau khi có 1 lượt tài trợ**

Trang web nhận tại trợ từ các nhà hảo tâm [light-up-hope.sosvietnam.org](https://light-up-hope.sosvietnam.org/)
- Cả 500 bóng đèn chớp kiểu pháo hoa chúc mừng trong vòng 15 giây, nhiều màu sắc, tạo cảm xúc hứng thú cho nhà tài trợ.
- Chỉ có 1 bóng của người vừa tài trợ chớp sáng trong vòng 2 phút (màu vàng)/
- Các bóng đèn khác (đã có tài trợ) từ từ sáng lên, giữ nguyên cho đến khi có nhà tài trợ tiếp theo.
- Các bóng đèn sẽ lần lượt được thắp sáng từ dưới gốc cây lên nếu được.

**1.2 Phương án back-up**

1 cái nút điều khiển để ấn 1 phát đèn sẽ chạy 1 loạt như các yêu cầu ở mục phía trên, trong trường hợp thông tin nhập vào website không được.

## 2. Triển khai

**2.1 Device**

- [x] [Board ESP8266-WiFi-Uno](https://iotmaker.vn/esp8266-iot-wifi-uno.html)
- [x] [LED ĐÚC 8MM F8 FULL COLOR XQD](http://ledxqd.com/shop/led-duc-xqd/led-duc-8mm-f8-full-color-xqd-ic-6803-9803-1903/)
- [x] [Apdapter 5V-3A](https://iotmaker.vn/ngun-cho-orange-pi-5v-3a.html)
- [ ] Mạch Chuyển Đổi Logic 3.3V-5.0V

**2.2 Lập trình**

1. Sử dụng board esp8266, dùng 1 nút nhấn để cấu hình, khi nhấn sẽ vào chế độ Wifimanager. Ở mục config sẽ add thêm 2 parameter là n (số lượng bóng led) và sever (đường dẫn url của sever). Server là của light-up-SOS, tạo trên máy tính và sử dụng Nodejs để test.
2. Về LED:
   * Khi chưa có kết nối wifi: Chạy hiệu ứng nhẹ mắt cho toàn bộ led (màu nền)
   * Khi có kết nối đến server, trả về 1 số n sau mỗi t giây (n là tổng số người donate). Nếu giá trị n hiện tại bằng giá trị trước đó nhận được từ sever => vẫn control led như cũ và không làm gì cả. Nếu giá trị n khác với gía trị củ thì thực hiện điều khiển như mục 1.1
   * Khi kết nối đến sever thất bại, thực hiện kết nối lại. sau 2 phút không kết nối được sẽ chạy hiệu ứng màu nền.
