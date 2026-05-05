# 📦 THIMPOS – PRODUCT API CONTRACT (v1.4)

**Base path**: "/api/products"

---

# 🧱 0. DATA MODEL (CORE DESIGN)

## 🧾 products

```sql
id           INTEGER PK
name         TEXT
price        INTEGER
description  TEXT
image_url    TEXT NULL
category     TEXT
is_active    BOOLEAN DEFAULT true

created_at   INTEGER (epoch)
updated_at   INTEGER (epoch)
````

---

# 1. ➕ CREATE PRODUCT

## `POST /api/products`

### Require Role

* `manager`

### Request JSON

```json
{
  "name": "Coca Cola",
  "price": 10000,
  "description": "Lon 330ml",
  "image_url": "https://cdn.example.com/coca.jpg",
  "category": "drink"
}
```

### Mapping

```cpp
co_await svc.createProduct(name, price, description, image_url, category);
```

### Response

```json
{
  "id": 123
}
```

### Status

* `201 Created`
* `400 Bad Request`

---

# 2. 🔍 GET PRODUCT BY ID

## `GET /api/products/{id}`

### Require Role

* `manager`, `staff`, `""`

### Query params

```
?active_only=true
```

### Mapping

```cpp
co_await svc.getProductById(id, active_only);
```

### Response

```json
{
  "id": 123,
  "name": "Coca Cola",
  "price": 10000,
  "description": "Lon 330ml",
  "image_url": "https://cdn.example.com/coca.jpg",
  "category": "drink",
  "is_active": true
}
```

### Status

* `200 OK`
* `404 Not Found`

---

# 3. 📦 LIST PRODUCTS (PAGING)

## `GET /api/products`

### Require Role

* `manager`, `staff`, `""`

### Query params

```
?limit=20
&offset=0
&active_only=true
```

### Mapping

```cpp
co_await svc.listProducts(limit, offset, active_only);
```

### Response

```json
{
  "data": [
    {
      "id": 1,
      "name": "Coca Cola",
      "price": 10000,
      "description": "Lon",
      "image_url": "https://cdn.example.com/coca.jpg",
      "category": "drink",
      "is_active": true
    }
  ],
  "paging": {
    "limit": 20,
    "offset": 0
  }
}
```

---

# 4. 🔎 SEARCH PRODUCTS (ENHANCED)

## `GET /api/products/search`

### Require Role

* `manager`, `staff`, `""`

### Query params

```
?q=coca
&category=drink
&from=1714000000
&to=1715000000
&limit=20
&offset=0
&active_only=true
```

### Params detail

| param         | type    | default | mô tả                           |
| ------------- | ------- | ------- | ------------------------------- |
| `q`           | string  | ""      | search theo `name` (LIKE `%q%`) |
| `category`    | string  | null    | filter chính xác theo category  |
| `from`        | integer | null    | `created_at >= from`            |
| `to`          | integer | null    | `created_at <= to`              |
| `limit`       | integer | 20      | số lượng                        |
| `offset`      | integer | 0       | phân trang                      |
| `active_only` | boolean | true    | chỉ lấy product active          |

### Mapping

```cpp
co_await svc.searchProducts(
    q,
    category,
    from,
    to,
    limit,
    offset,
    active_only
);
```

### Response

```json
{
  "data": [
    {
      "id": 1,
      "name": "Coca Cola",
      "price": 10000,
      "description": "Lon",
      "image_url": "https://cdn.example.com/coca.jpg",
      "category": "drink",
      "is_active": true
    }
  ],
  "paging": {
    "limit": 20,
    "offset": 0
  }
}
```

### Behavior

* Không truyền `category` → bỏ filter
* Không truyền `from/to` → không filter thời gian
* Có thể combine tất cả filter
* Backward compatible 100%

---

# 5. ✏️ UPDATE PRODUCT

## `PUT /api/products/{id}`

### Require Role

* `manager`

### Request JSON

```json
{
  "name": "Coca Cola Zero",
  "price": 12000,
  "description": "Lon 330ml",
  "image_url": "https://cdn.example.com/zero.jpg",
  "category": "drink",
  "is_active": true
}
```

### Mapping

```cpp
co_await svc.updateProduct(
    id,
    name,
    price,
    description,
    image_url,
    category,
    isActive
);
```

### Response

```json
{
  "success": true
}
```

---

# 6. 🗑️ DELETE PRODUCT (SOFT DELETE)

## `DELETE /api/products/{id}`

### Require Role

* `manager`

### Mapping

```cpp
co_await svc.deleteProduct(id);
```

### Response

```json
{
  "success": true
}
```

---

# 🔐 7. ROLE ENFORCEMENT SUMMARY

| Endpoint                  | manager | staff | "" |
| ------------------------- | ------- | ----- | -- |
| POST /api/products        | ✔       | ✖     | ✖  |
| GET /api/products/{id}    | ✔       | ✔     | ✔  |
| GET /api/products         | ✔       | ✔     | ✔  |
| GET /api/products/search  | ✔       | ✔     | ✔  |
| PUT /api/products/{id}    | ✔       | ✖     | ✖  |
| DELETE /api/products/{id} | ✔       | ✖     | ✖  |
