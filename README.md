# 🌐 🏠 HỆ THỐNG NHÀ THÔNG MINH QUA GIAO TIẾP KHÔNG DÂY WIFI MESH VỚI ESP32

🎯 Mô tả ngắn gọn:

Dự án "Hệ thống nhà thông minh sử dụng ESP32 & Wifi Mesh" là một giải pháp IoT ứng dụng trong nông nghiệp, cho phép giám sát các thông số môi trường như nhiệt độ, độ ẩm không khí (DHT11), cường độ ánh sáng và chuyển động (HCSR501. Hệ thống sử dụng giao tiếp Wifi Mesh không dây giữa các node cảm biến ESP32 và ESP32 Gateway, nơi dữ liệu được hiển thị qua Web Server. Người dùng có thể quan sát thông tin môi trường thời gian thực và điều khiển thiết bị  từ xa qua trình duyệt web.



## 📖 Mục lục

- [🚀 Giới thiệu](#-giới-thiệu)
- [🏗️ Kiến trúc hệ thống](#%EF%B8%8F-kiến-trúc-hệ-thống)
- [⚙️ Thông số kỹ thuật](#️-thông-số-kỹ-thuật)
- [🧰 Danh sách linh kiện](#-danh-sách-linh-kiện)
- [🛠️ Hướng dẫn lắp ráp](#️-hướng-dẫn-lắp-ráp)
- [🧠 Lập trình firmware](#-lập-trình-firmware)
- [🌐 Cách sử dụng](#-cách-sử-dụng)
- [✅ Kiểm thử](#-kiểm-thử)
- [📸 Ảnh / Video demo](#-ảnh--video-demo)
- [🤝 Đóng góp](#-đóng-góp)
- [📄 Giấy phép](#-giấy-phép)

---

## 🚀 Giới thiệu

📌 Tóm tắt dự án
Dự án: Hệ thống nhà thông minh không dây sử dụng ESP32 và giao tiếp Wifi Mesh.

Chức năng chính:
Giám sát và điều khiển từ xa các thông số môi trường như nhiệt độ, độ ẩm không khí, ánh sáng và chuyển động của con người thông qua giao diện Web. Hệ thống có thể điều khiển thiết bị như bật/tắt đèn.

Người dùng chính:

Các hộ gia đình, các tòa chung cư
Mục tiêu:

Giáo dục – học thuật: hỗ trợ học tập và nghiên cứu trong lĩnh vực IoT, điều khiển nhúng, và nhà thông minh.

Ứng dụng thực tiễn: phục vụ các mô hình gia đình nhỏ.

Tiềm năng thương mại: có thể phát triển thành sản phẩm thực tế với chi phí thấp và khả năng mở rộng


## 🏗️ Kiến trúc hệ thống

ESP32 Node 1 (Cảm biến) ──> ESP32 Node 2 (Trung gian) ──> ESP32 3 (Nhận dữ liệu và truyền qua uart) ──> ESP32 4 (Gateway)  <──> Trình duyệt Web                               

## ⚙️ Thông số kỹ thuật

| Thông số               | Chi tiết                                  |
|------------------------|--------------------------------------------|
| 🔧 Vi điều khiển         | ESP32 Dev Module                           |
| 📶 Giao tiếp không dây   | Wifi Mesh      |
| 🌐 Web Server            | ESP32 WebServer (HTML nhúng)              |
| 🌡️ Cảm biến tích hợp      | DHT11, HCSR501,                             |
| 🔋 Nguồn cấp              | 5V DC (USB hoặc nguồn ngoài)              |
| 🔗 Số Slave hỗ trợ       | 1 (có thể mở rộng)                         |
| 💡 Điều khiển            | LED        |

---

## 🧰 Danh sách linh kiện

| 🔩 Linh kiện                   | Số lượng |
|-------------------------------|----------|
| ESP32 Dev Module              | 4        |
| Cảm biến DHT11                | 1        |
| Cảm biến HCSR501              | 1        |
| LED + điện trở 220Ω           | 3        |
| Breadboard và dây nối         | Nhiều    |
| Nguồn 5V hoặc pin sạc         | Tùy chọn |

---

## 🛠️ Hướng dẫn lắp ráp

1.  Nối module cảm biến với ESP 1 với chân GPIO phù hợp.
2.  Cấp nguồn cho ESP 2.
3.  Gắn LED và điện trở vào các chân GPIO phù hợp của ESP 3.
4.  Kết nối ESP 3 với ESP 4 bằng UART.
5.   Nạp code để chạy thử.

---

## 🧠 Lập trình firmware

### 📁 Các tệp chính:

| File                    | Mô tả                                                               |
|-------------------------|---------------------------------------------------------------------|
| `GUIDL.ino` | Đọc dữ liệu cảm biến gửi tới ESP trung gian qua wifi mesh    |
| `ESPTG.ino`            | Nhận dữ liệu từ ESP cảm biến và gửi tới ESP 3 qua wifi mesh          |
| `ESP 3.ino`            | Nhận dữ liệu từ ESp trung gian và gửi tới Gateway qua UART         |
| `ESP 4.ino`            | Nhận dữ liệu và truyền tới mqtt        |
| `app.py`               | Truy vấn và xử lý dữ liệu          |
| `mqtt.py`              | dùng mqtt để đưa dữ liệu nhận được vào mysql         |
| `main.h`               | Giao diện Web (HTML, CSS, JS nhúng trực tiếp trong code)           |

### 📚 Thư viện Arduino cần cài đặt:

- `painlessMesh.h`
- `DHT.h`
- `HardwareSerial.h`
- `ArduinoJson.h`
- `PubSubClient.h`
- `WiFi.h`
---

## 🌐 Cách sử dụng

1.  Cấp nguồn cho tất cả ESP32 (Master + Slave).
2.  Chạy khởi động MQTT.
3.  Chạy file mqtt.py và app.py và chạy Web.
4.  Quan sát dữ liệu từ Slave: Nhiệt độ, độ ẩm, ánh sáng, chuyển động.
5.  Điều khiển bật/tắt LED từ giao diện Web.

## ✅ Kiểm thử

Hệ thống đã được kiểm thử và hoạt động ổn định với các chức năng chính như sau:

📡 Truyền dữ liệu : thiết bị Slave truyền dữ liệu cảm biến ( DHT11, HCSR501) về thiết bị Master một cách chính xác và liên tục.

🔁 Nhận dữ liệu: Thiết bị Master nhận đầy đủ và kịp thời dữ liệu từ node Slave thông qua giao tiếp Wifi Mesh và UART.

🌐 Giao diện Web: Giao diện Web Server trên ESP32 Master hiển thị chính xác các thông số môi trường:

🌡️ Nhiệt độ

💧 Độ ẩm không khí

☀️ Cường độ ánh sáng

💡 Điều khiển thiết bị: Chức năng bật/tắt đèn hoạt động tốt, phản hồi thực tế nhanh chóng sau khi thao tác trên Web.


🔒 Độ ổn định: Hệ thống hoạt động liên tục và ổn định trong các thử nghiệm thực tế, không xảy ra treo hoặc mất kết nối.

---

## 📸 Ảnh / Video demo
  
- Demo kết quả
  🔒 Dữ liệu được hiển thị lên webwerver và điều khiển các thiết bị từ xa
  ![IMAGE](https://github.com/user-attachments/assets/492ee04c-c811-4ca7-96b1-71ede1ccf0ce)

  🌐 VIDEO DEMO
  [h.VIDEO DEMO](https://youtu.be/cQoYE9cn7Jk)

- Log dữ liệu chạy trong nhiều ngày liên tục
 ![IMAGE](https://github.com/user-attachments/assets/bd6f2118-c3f5-44b2-9210-2bd205c6baf9)

---

## 🤝 Đóng góp

Bạn có ý tưởng cải tiến hoặc phát hiện lỗi?  
📬 Hãy gửi issue hoặc pull request — **mọi đóng góp đều được chào đón!**

---

## 📄 Giấy phép

📜 Dự án được phát hành theo giấy phép [MIT License](LICENSE).  
Bạn có toàn quyền sử dụng, chỉnh sửa và phân phối lại với điều kiện giữ nguyên thông tin giấy phép.

---
## ✍️ Tác giả
Dự án được thực hiện bởi nhóm sinh viên:

👨‍💻 Phạm Đức Mạnh

👨‍💻 Phạm Tuấn Anh

👨‍💻 Phạm Hoài Nam

👨‍💻 Lê Tuấn Anh

Xin chân thành cảm ơn sự hỗ trợ và hướng dẫn từ giảng viên và nhà trường trong suốt quá trình thực hiện dự án.
