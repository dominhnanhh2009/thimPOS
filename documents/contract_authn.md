# 🔐 AuthN HTTP API Contract (1.5)

## 1. 🎯 Scope

Module AuthN chịu trách nhiệm:

* Đăng ký user
* Đăng nhập / đăng xuất
* Quản lý session (multi-device)
* Resolve user từ session token

❗ Không xử lý role / permission (AuthZ tách riêng hoặc nằm ở controller/service khác)

---

## 2. 🍪 Session Model

### Session Strategy

* Session-based authentication
* Token lưu trong cookie:
```
Cookie: sid=<session_token>
```

### Server behavior (AuthNFilter)

Nếu cookie hợp lệ:

* resolve username
* attach vào request context:
```
request->setAttribute("username", ...)
```

Nếu không hợp lệ:

* request không có username (unauthenticated)

---

## 3. 📡 Base Path
```
/auth
```

---

## 4. 🔑 API Endpoints

---

## 4.1 Register

### POST `/auth/register` (no required role)

Tạo user mới.

### Request
```json
{
  "username": "string",
  "password": "string"
}
```

### Response

#### 200 OK
```json
{
  "success": true
}
```

#### 409 Conflict

```json
{
  "success": false,
  "error": "USER_EXISTS"
}
```

---

## 4.2 Login (no required role)

### POST `/auth/login`

Tạo session và trả cookie.

### Request
```json
{
  "username": "string",
  "password": "string"
}
```

### Response

#### 200 OK

```
Set-Cookie: sid=<session_token>; HttpOnly; Path=/
```

```json
{
  "success": true,
  "username": "string"
}
```

#### 401 Unauthorized

```json
{
  "success": false,
  "error": "INVALID_CREDENTIALS"
}
```

---

## 4.3 Logout (current session)

### POST `/auth/logout`

Xóa session hiện tại.

### Auth Required

✔ Cookie `sid`

### Response
```json
{
  "success": true
}
```

---

## 4.4 Logout All Sessions

### POST `/auth/logout_all`

Invalidate toàn bộ session của user.

### Auth Required

✔ Cookie `sid`

### Response
```json
{
  "success": true,
  "invalidated_sessions": 3
}
```

---

## 4.5 Get Current User

### GET `/auth/me`

### Auth Required

✔ Cookie `sid`

### Response

#### 200 OK
```json
{
  "authenticated": true,
  "username": "string"
}
```

#### 401 Unauthorized

```json
{
  "authenticated": false
}
```

---

## 4.6 Get User Sessions

### GET `/auth/sessions`

Liệt kê session hiện có của user.

### Auth Required

✔ Cookie `sid`

### Response
```json
{
  "sessions": [
    "token_1",
    "token_2"
  ]
}
```

---

## 4.7 Delete Specific Session

### DELETE `/auth/sessions/{token}`

Xóa 1 session cụ thể.

### Auth Required

✔ Cookie `sid`

### Response
```json
{
  "success": true
}
```

---

## 4.8 Get All Users (required role: manager)

### GET `/auth/users`

> Dùng cho internal/admin tooling nhẹ

### Response
```json
{
  "users": [
    "alice",
    "bob",
    "charlie"
  ]
}
```

---

## 4.9 Change Password (required role: manager)

### POST `/auth/change_password`

Đổi password user.

### Request
```json
{
  "username": "string",
  "new_password": "string"
}
```

### Response

#### 200 OK
```json
{
  "success": true
}
```

#### 404 Not Found

```json
{
  "success": false,
  "error": "USER_NOT_FOUND"
}
```

---

## 4.10 Delete User Credentials (required role: manager)

### DELETE `/auth/users/{username}`

Xóa hoàn toàn credentials của một user. Thao tác này sẽ xóa bản ghi trong bảng credentials và thu hồi toàn bộ session liên quan.

### Response

#### 200 OK
```json
{
  "success": true
}
```

#### 404 Not Found
```json
{
  "success": false,
  "error": "USER_NOT_FOUND"
}
```

---

## 5. 🔐 Error Model (Unified)
```json
{
  "success": false,
  "error": "ERROR_CODE"
}
```

### Common codes

| Code                | Meaning               |
| ------------------- | --------------------- |
| INVALID_CREDENTIALS | sai username/password |
| USER_EXISTS         | username đã tồn tại   |
| UNAUTHORIZED        | thiếu/invalid session |
| USER_NOT_FOUND      | user không tồn tại    |
| INTERNAL_ERROR      | lỗi server            |
|FORBIDDEN            | ko có quyền           |

---

## 6. 🧠 AuthNFilter Contract

### Input

* Cookie: `sid`

### Output (request context)

If valid:
```
username = "alice"
```

Else:
no attribute (with 401 response + error json)