# Kiểm tra license khi khởi chạy

ThimPOS mặc định kích hoạt key với KeyManager Cloud trước khi mở HTTP server. Client kiểm tra chữ ký ES256, product `thimpos`, trạng thái, ngày bắt đầu, ngày hết hạn và cửa sổ offline. License đã ký được lưu ở `.thimpos-license.json`; lần chạy sau có thể dùng cache offline đến `offline_until`.

## Chạy lần đầu

Linux/macOS:

```sh
THIMPOS_LICENSE_KEY='<license-key>' ./ThimPOS
```

PowerShell:

```powershell
$env:THIMPOS_LICENSE_KEY = '<license-key>'
./ThimPOS.exe
```

Các biến tùy chọn:

- `THIMPOS_DEVICE_ID`: ID thiết bị ổn định do hệ thống triển khai cung cấp. Nếu bỏ trống, client tự tạo và giữ trong cache.
- `THIMPOS_LICENSE_CACHE`: đổi đường dẫn cache license.
- `THIMPOS_KEY_MANAGER_URL`: đổi API URL, chủ yếu phục vụ kiểm thử.

Không commit key hoặc file cache. Khi KeyManager trả lỗi license (hết hạn, bị thu hồi, sai product hoặc hết giới hạn), server sẽ không khởi chạy. Cache chỉ được dùng thay thế khi không kết nối được API và bản license đã ký vẫn còn trong cửa sổ offline.

## Build nội bộ không kiểm tra key

Kiểm tra key được bật mặc định:

```sh
cmake -S . -B build
```

Chỉ tắt rõ ràng cho build phát triển/nội bộ:

```sh
cmake -S . -B build -DTHIMPOS_IGNORE_KEY_CHECK=ON
```

Binary kiểu này in cảnh báo lúc khởi chạy và không chứa client verifier.
