# Hướng dẫn sử dụng ThimPOS

Tài liệu này dành cho người sử dụng trực tiếp trên giao diện ThimPOS.

## Đăng nhập và đăng ký

- Tại màn hình **Đăng nhập**, nhập tên đăng nhập và mật khẩu rồi chọn **Đăng nhập**.
- Nếu chưa có tài khoản, chọn **Đăng ký ngay**, nhập tên đăng nhập và mật khẩu, sau đó chọn **Tạo tài khoản**.
- Khi hoàn tất công việc, chọn **Đăng xuất** trên thanh điều hướng.

## Nhân viên bán hàng

### Tạo đơn tại màn hình ThimPOS

1. Tìm sản phẩm theo tên hoặc chọn nhóm sản phẩm.
2. Chọn sản phẩm để thêm vào **Đơn hiện tại**.
3. Điều chỉnh số lượng trong đơn; chọn dấu **×** để bỏ một sản phẩm.
4. Kiểm tra tổng tiền.
5. Chọn **THANH TOÁN** để tạo đơn, hoặc **Xóa** để làm trống đơn hiện tại.

### Quản lý đơn hàng

- Chọn **Order CRUD** trong menu nhân viên.
- Ở khu vực **Create Order**, thêm từng sản phẩm và số lượng, rồi chọn **Create**. Chọn **Reset** để nhập lại.
- Ở khu vực **Orders**, chọn **Reload** để tải danh sách đơn. Có thể thay đổi số lượng bản ghi và vị trí bắt đầu trước khi tải.
- Chọn một đơn trong danh sách hoặc nhập mã đơn tại **Detail**, rồi chọn **Load** để xem chi tiết.
- Trong chi tiết đơn, có thể chọn **Edit** để sửa, **Save** để lưu, **Print** để in hóa đơn hoặc **Delete** để xóa đơn.

## Quản lý

### Sản phẩm

- Chọn **Products** trong menu quản lý.
- Để tạo sản phẩm, nhập tên, giá, mô tả, nhóm sản phẩm và đường dẫn ảnh, rồi chọn **Tạo**.
- Dùng khu vực **Search** để tìm theo từ khóa, nhóm hoặc lọc sản phẩm đang hoạt động. Chọn **Reset** để trở về danh sách đầy đủ.
- Có thể sửa trực tiếp thông tin trong danh sách và chọn biểu tượng **💾** để lưu.
- Chọn biểu tượng **🗑️** để xóa sản phẩm.

### Báo cáo bán hàng

- Chọn **báo cáo sale** trong menu quản lý.
- Chọn khoảng thời gian cụ thể hoặc một mốc có sẵn: hôm nay, tuần này, tháng này, năm nay hoặc tất cả.
- Chọn **Tạo báo cáo** để xem biểu đồ doanh thu, doanh thu theo sản phẩm, các thống kê nổi bật và bảng sản phẩm đã bán.
- Chọn tiêu đề cột trong bảng để thay đổi cách sắp xếp.

### Tài khoản nhân viên

- Chọn **Quản lý tài khoản nhân viên** trong menu quản lý.
- Tạo hồ sơ bằng cách nhập tên đăng nhập, vai trò, email, số điện thoại, họ tên và ngày sinh, rồi chọn **Create**.
- Trong danh sách tài khoản, chọn **Edit** để sửa hoặc **Delete** để xóa hồ sơ.
- Tại khu vực thông tin đăng nhập, chọn **Tạo username/password** để cấp tài khoản; có thể đổi mật khẩu hoặc xóa thông tin đăng nhập của tài khoản đã có.
- Tại khu vực **Sessions**, chọn **Load My Sessions** để xem các phiên đăng nhập của mình và **kill** để kết thúc một phiên.

## Lưu ý khi thao tác

### Xóa license đã lưu

Chỉ thực hiện khi cần nhập lại license, đổi key hoặc khi bộ phận hỗ trợ yêu cầu:

1. Đóng ThimPOS nếu chương trình đang chạy.
2. Mở PowerShell tại thư mục chứa `ThimPOS.exe`.
3. Chạy `./ThimPOS.exe --clear-license-cache`.
4. Khi thấy thông báo **Đã xóa cache bản quyền**, mở ThimPOS lại và nhập license key.

Lệnh này chỉ xóa thông tin license được lưu trên máy, không xóa sản phẩm, đơn hàng hay tài khoản. Hiện tại thao tác này không gửi yêu cầu hủy phiên license lên máy chủ.

- Kiểm tra lại sản phẩm, số lượng và tổng tiền trước khi thanh toán hoặc in hóa đơn.
- Các thao tác xóa không thể sử dụng lại dữ liệu đã xóa; chỉ xác nhận khi đã kiểm tra đúng đối tượng.
- Nếu giao diện báo mất kết nối hoặc tải dữ liệu thất bại, kiểm tra kết nối rồi thử lại. Nếu lỗi vẫn còn, liên hệ người quản lý hệ thống.
