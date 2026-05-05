Mục tiêu: **viết template hóa đơn (.tpl) mà không cần đụng vào C++**

---

# 🧾 MiniDoc — Template Engine (Bill Renderer)

## 1. 📦 Tổng quan

Engine này dùng để render hóa đơn từ file `.tpl` với cú pháp:

```
{{key}}
{{key|width}}
{{key|format}}
{{#items}} ... {{/items}}
```

Pipeline chạy như sau:

```
.tpl → parse → inject data → output string
```

---

## 2. 🧱 Biến cơ bản

### ✔️ Cú pháp

```
{{field}}
```

### ✔️ Ví dụ

```
Mã đơn: {{id}}
Tổng tiền: {{total_price}}
```

---

## 3. 📏 Padding (canh lề)

### ✔️ Cú pháp

```
{{field|width}}
```

* Cắt nếu dài hơn
* Thêm space nếu ngắn hơn
* Align trái

### ✔️ Ví dụ

```
{{name|20}}{{qty|5}}{{price|10}}
```

👉 Output kiểu:

```
Bánh mì           2    15000
```

---

## 4. ⏱️ Format thời gian

### ✔️ Cú pháp

```
{{timestamp|format}}
```

### ✔️ Special format

```
datetime → %Y-%m-%d %H:%M:%S
```

### ✔️ Ví dụ

```
Ngày tạo: {{created_at|datetime}}
```

hoặc custom:

```
{{created_at|%d/%m/%Y}}
```

---

## 5. 🔁 Loop items (danh sách sản phẩm)

### ✔️ Cú pháp

```
{{#items}}
... block ...
{{/items}}
```

### ✔️ Available fields trong loop

| field      | mô tả        |
| ---------- | ------------ |
| name       | tên sản phẩm |
| qty        | số lượng     |
| price      | giá          |
| line_total | tổng dòng    |

---

### ✔️ Ví dụ

```
{{#items}}
{{name|20}}{{qty|5}}{{price|10}}{{line_total|10}}
{{/items}}
```

👉 Render:

```
Coca               2     10000     20000
Bánh mì           1     15000     15000
```

---

## 6. 🧩 Biến tổng thể (root)

Ngoài items, bạn có thể dùng:

| field       | mô tả                  |
| ----------- | ---------------------- |
| id          | mã đơn                 |
| created_at  | timestamp              |
| updated_at  | timestamp              |
| total_price | tổng tiền              |
| items       | (auto render từ block) |

---

## 7. 🧾 Ví dụ template hoàn chỉnh

```txt
=========== HÓA ĐƠN ===========
Mã đơn: {{id}}
Ngày: {{created_at|datetime}}

--------------------------------
Tên                SL   Giá     Tổng
--------------------------------

{{#items}}
{{name|18}}{{qty|5}}{{price|8}}{{line_total|10}}
{{/items}}

--------------------------------
TỔNG: {{total_price}}

Cảm ơn quý khách!
```

---

## 8. ⚠️ Lưu ý quan trọng

### ❗ 1. Không có nested loop

```
{{#items}} ... {{#something}} ❌
```

### ❗ 2. Không có if/else

→ Nếu cần thì xử lý trước ở C++

### ❗ 3. Timestamp phải là số

```
"1715000000" ✔️
"2024-01-01" ❌
```

---

## 9. 💡 Tips “pro”

### 🔹 Căn bảng đẹp

```
{{name|20}}{{qty|5}}{{price|10}}
```

### 🔹 Fake separator

```
--------------------------------
```

### 🔹 Tối ưu đọc

* Giữ template càng “text-like” càng tốt
* Tránh logic phức tạp trong tpl

---

## 10. 🚀 Triết lý thiết kế

Engine này cố tình:

* ❌ Không logic phức tạp

* ❌ Không condition

* ❌ Không DSL

* ✅ Nhanh

* ✅ Dễ debug

* ✅ Plug-and-play

