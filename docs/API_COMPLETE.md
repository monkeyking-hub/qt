# 航班管理系统 API 完整文档

## 基本信息

- **Base URL**: `https://api.flightsystem.com/v1`
- **协议**: HTTPS
- **认证**: Bearer Token (JWT)
- **响应格式**: JSON
- **请求超时**: 30秒
- **版本**: v1.0

## 通用规范

### 请求头

```
Content-Type: application/json
User-Agent: FlightSystem/1.0
Authorization: Bearer {token}  // 需要认证的请求必须包含
```

### 响应格式

成功响应 (HTTP 200):
```json
{
  "code": 0,
  "message": "success",
  "data": {},
  "timestamp": "2024-01-20T10:30:00Z"
}
```

错误响应:
```json
{
  "code": 4000,
  "message": "error description",
  "errors": [
    {
      "field": "field_name",
      "message": "error message"
    }
  ],
  "timestamp": "2024-01-20T10:30:00Z"
}
```

### 状态码

| 代码 | 说明 |
|------|------|
| 0    | 操作成功 |
| 4000 | 客户端错误 |
| 4001 | 未授权 |
| 4002 | 资源不存在 |
| 4003 | 禁止访问 |
| 5000 | 服务器错误 |

## 认证相关 API

### 1. 用户登录
**POST** `/auth/login`

**权限**: 无需认证

**请求体**:
```json
{
  "username": "user@example.com",
  "password": "password123"
}
```

**响应**:
```json
{
  "code": 0,
  "message": "Login successful",
  "data": {
    "user_id": "user_123",
    "token": "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9...",
    "refresh_token": "refresh_token_here",
    "expires_in": 3600,
    "user": {
      "user_id": "user_123",
      "username": "john_doe",
      "email": "user@example.com",
      "full_name": "John Doe",
      "avatar_url": "https://...",
      "role": "customer"
    }
  }
}
```

---

### 2. 用户注册
**POST** `/auth/register`

**权限**: 无需认证

**请求体**:
```json
{
  "email": "newuser@example.com",
  "username": "newuser",
  "password": "password123",
  "confirm_password": "password123",
  "full_name": "New User",
  "phone": "+86 13800000000"
}
```

**响应**:
```json
{
  "code": 0,
  "message": "Registration successful",
  "data": {
    "user_id": "user_456",
    "email": "newuser@example.com",
    "username": "newuser",
    "message": "Welcome! Please verify your email."
  }
}
```

---

### 3. 退出登录
**POST** `/auth/logout`

**权限**: 需要认证

**响应**:
```json
{
  "code": 0,
  "message": "Logout successful"
}
```

---

### 4. 刷新令牌
**POST** `/auth/refresh`

**权限**: 需要认证

**请求头**:
```
Authorization: Bearer {refresh_token}
```

**响应**:
```json
{
  "code": 0,
  "message": "Token refreshed",
  "data": {
    "token": "new_token_here",
    "expires_in": 3600
  }
}
```

---

## 航班相关 API

### 1. 搜索航班
**POST** `/flights/search`

**权限**: 无需认证

**请求体**:
```json
{
  "departure": "PEK",
  "destination": "SHA",
  "date": "2024-02-15",
  "return_date": "2024-02-20",
  "passengers": 1,
  "seat_class": "economy"
}
```

**查询参数**:
- `departure`: 出发地代码 (必需)
- `destination`: 目的地代码 (必需)
- `date`: 出发日期, 格式 YYYY-MM-DD (必需)
- `return_date`: 返回日期 (可选, 不提供表示单程)
- `passengers`: 乘客数 (默认1)
- `seat_class`: 座位等级 - economy/business/first (默认economy)

**响应**:
```json
{
  "code": 0,
  "message": "Search successful",
  "data": {
    "flights": [
      {
        "flight_number": "CA123",
        "airline": "Air China",
        "airline_code": "CA",
        "aircraft_type": "Boeing 737",
        "departure_airport": "PEK",
        "arrival_airport": "SHA",
        "departure_time": "2024-02-15T10:00:00Z",
        "arrival_time": "2024-02-15T12:30:00Z",
        "duration_minutes": 150,
        "available_seats": {
          "economy": 45,
          "business": 12,
          "first": 3
        },
        "price": {
          "economy": 680.00,
          "business": 1680.00,
          "first": 2680.00
        },
        "status": "on_time",
        "stops": 0,
        "rating": 4.5,
        "reviews": 230
      }
    ],
    "search_id": "search_123456",
    "total_flights": 1
  }
}
```

---

### 2. 获取航班详情
**GET** `/flights/{flight_number}`

**权限**: 无需认证

**路径参数**:
- `flight_number`: 航班号 (如 CA123)

**响应**:
```json
{
  "code": 0,
  "message": "Flight details retrieved",
  "data": {
    "flight_number": "CA123",
    "airline": "Air China",
    "airline_logo": "https://...",
    "aircraft_type": "Boeing 737-800",
    "registration": "B-5238",
    "departure_airport": "Beijing Capital International Airport (PEK)",
    "arrival_airport": "Shanghai Pudong International Airport (SHA)",
    "departure_time": "2024-02-15T10:00:00Z",
    "arrival_time": "2024-02-15T12:30:00Z",
    "duration_minutes": 150,
    "status": "scheduled",
    "terminal": "T1",
    "gate": "A12",
    "seat_map": "6x100",
    "amenities": [
      "wifi",
      "meals",
      "entertainment",
      "power_outlet"
    ],
    "baggage_allowance": {
      "economy": "23kg + 7kg",
      "business": "32kg + 8kg",
      "first": "50kg + 8kg"
    },
    "fare_rules": {
      "refundable": true,
      "changeable": true,
      "cancellation_penalty": 0
    }
  }
}
```

---

### 3. 获取航班日程
**GET** `/flights/schedule?from={from}&to={to}`

**权限**: 无需认证

**查询参数**:
- `from`: 出发地代码 (必需)
- `to`: 目的地代码 (必需)

**响应**:
```json
{
  "code": 0,
  "message": "Schedule retrieved",
  "data": {
    "schedule": [
      {
        "date": "2024-02-15",
        "flights": [
          {
            "flight_number": "CA123",
            "departure_time": "10:00",
            "arrival_time": "12:30",
            "available_seats": 45,
            "price": 680.00
          }
        ]
      }
    ]
  }
}
```

---

### 4. 获取可用座位
**GET** `/flights/{flight_number}/seats`

**权限**: 无需认证

**响应**:
```json
{
  "code": 0,
  "message": "Seats retrieved",
  "data": {
    "seats": [
      {
        "seat_id": "1A",
        "seat_number": "1A",
        "row": 1,
        "column": "A",
        "class": "economy",
        "status": "available",
        "price_adjustment": 0,
        "is_emergency_exit": false,
        "is_window": true
      }
    ],
    "total_seats": 180,
    "available_seats": 45
  }
}
```

---

### 5. 获取航班评价
**GET** `/flights/{flight_number}/reviews`

**权限**: 无需认证

**查询参数**:
- `page`: 页码 (默认1)
- `limit`: 每页数量 (默认10)

**响应**:
```json
{
  "code": 0,
  "message": "Reviews retrieved",
  "data": {
    "reviews": [
      {
        "review_id": "review_123",
        "user_id": "user_123",
        "username": "john_doe",
        "avatar_url": "https://...",
        "rating": 4.5,
        "title": "Great flight experience",
        "content": "The flight was on time and the service was excellent.",
        "verified_purchase": true,
        "helpful_count": 23,
        "created_at": "2024-01-20T10:30:00Z"
      }
    ],
    "average_rating": 4.5,
    "total_reviews": 230,
    "page": 1,
    "pages": 3
  }
}
```

---

## 预订相关 API

### 1. 创建预订
**POST** `/bookings`

**权限**: 需要认证

**请求体**:
```json
{
  "flight_number": "CA123",
  "search_id": "search_123456",
  "passengers": [
    {
      "passenger_type": "adult",
      "first_name": "John",
      "last_name": "Doe",
      "gender": "male",
      "date_of_birth": "1990-01-01",
      "nationality": "CN",
      "passport_number": "G12345678",
      "passport_expiry": "2025-12-31",
      "email": "john@example.com",
      "phone": "+86 13800000000"
    }
  ],
  "seats": ["1A", "1B"],
  "services": {
    "meal": true,
    "seat_selection": true,
    "baggage": false,
    "insurance": true
  },
  "contact_info": {
    "email": "john@example.com",
    "phone": "+86 13800000000"
  }
}
```

**响应**:
```json
{
  "code": 0,
  "message": "Booking created successfully",
  "data": {
    "booking_id": "BK20240120001",
    "booking_reference": "ABCD1234",
    "status": "pending_payment",
    "flight_number": "CA123",
    "departure_time": "2024-02-15T10:00:00Z",
    "arrival_time": "2024-02-15T12:30:00Z",
    "passengers": [
      {
        "booking_passenger_id": "bp_123",
        "first_name": "John",
        "last_name": "Doe",
        "seat": "1A"
      }
    ],
    "price_breakdown": {
      "base_price": 680.00,
      "taxes": 50.00,
      "fees": 10.00,
      "services": 100.00,
      "discount": 0,
      "total": 840.00
    },
    "expires_at": "2024-01-20T12:30:00Z"
  }
}
```

---

### 2. 获取预订详情
**GET** `/bookings/{booking_id}`

**权限**: 需要认证

**响应**:
```json
{
  "code": 0,
  "message": "Booking details retrieved",
  "data": {
    "booking_id": "BK20240120001",
    "booking_reference": "ABCD1234",
    "status": "confirmed",
    "created_at": "2024-01-20T10:00:00Z",
    "flight_number": "CA123",
    "departure_time": "2024-02-15T10:00:00Z",
    "arrival_time": "2024-02-15T12:30:00Z",
    "passengers": [
      {
        "booking_passenger_id": "bp_123",
        "first_name": "John",
        "last_name": "Doe",
        "seat": "1A",
        "status": "checked_in"
      }
    ],
    "price_breakdown": {
      "base_price": 680.00,
      "taxes": 50.00,
      "total": 840.00
    },
    "payment_status": "paid",
    "payment_method": "credit_card",
    "check_in_url": "https://..."
  }
}
```

---

### 3. 取消预订
**POST** `/bookings/{booking_id}/cancel`

**权限**: 需要认证

**请求体** (可选):
```json
{
  "reason": "Change of plans",
  "refund_method": "original_payment"
}
```

**响应**:
```json
{
  "code": 0,
  "message": "Booking cancelled successfully",
  "data": {
    "booking_id": "BK20240120001",
    "status": "cancelled",
    "cancellation_reason": "Change of plans",
    "refund_amount": 750.00,
    "refund_status": "processing",
    "estimated_refund_date": "2024-01-27"
  }
}
```

---

### 4. 获取预订历史
**GET** `/users/{user_id}/bookings`

**权限**: 需要认证

**查询参数**:
- `page`: 页码 (默认1)
- `limit`: 每页数量 (默认10)
- `status`: 筛选状态 - confirmed/cancelled/completed (可选)

**响应**:
```json
{
  "code": 0,
  "message": "Bookings retrieved",
  "data": {
    "bookings": [
      {
        "booking_id": "BK20240120001",
        "booking_reference": "ABCD1234",
        "flight_number": "CA123",
        "status": "confirmed",
        "departure_time": "2024-02-15T10:00:00Z",
        "arrival_time": "2024-02-15T12:30:00Z",
        "total_price": 840.00,
        "created_at": "2024-01-20T10:00:00Z"
      }
    ],
    "page": 1,
    "pages": 1,
    "total": 1
  }
}
```

---

## 用户相关 API

### 1. 获取用户个人信息
**GET** `/users/{user_id}`

**权限**: 需要认证

**响应**:
```json
{
  "code": 0,
  "message": "User profile retrieved",
  "data": {
    "user_id": "user_123",
    "username": "john_doe",
    "email": "john@example.com",
    "full_name": "John Doe",
    "phone": "+86 13800000000",
    "avatar_url": "https://...",
    "gender": "male",
    "date_of_birth": "1990-01-01",
    "nationality": "CN",
    "address": "123 Main Street",
    "city": "Beijing",
    "state": "Beijing",
    "postal_code": "100000",
    "country": "China",
    "role": "customer",
    "status": "active",
    "created_at": "2023-01-01T00:00:00Z",
    "updated_at": "2024-01-20T10:30:00Z"
  }
}
```

---

### 2. 更新用户个人信息
**PUT** `/users/{user_id}`

**权限**: 需要认证

**请求体**:
```json
{
  "full_name": "John Doe",
  "phone": "+86 13800000000",
  "gender": "male",
  "date_of_birth": "1990-01-01",
  "address": "123 Main Street",
  "city": "Beijing",
  "postal_code": "100000"
}
```

**响应**:
```json
{
  "code": 0,
  "message": "User profile updated",
  "data": {
    "user_id": "user_123",
    "message": "Profile updated successfully"
  }
}
```

---

### 3. 获取用户收藏
**GET** `/users/{user_id}/favorites`

**权限**: 需要认证

**响应**:
```json
{
  "code": 0,
  "message": "Favorites retrieved",
  "data": {
    "favorites": [
      {
        "favorite_id": "fav_123",
        "flight_number": "CA123",
        "airline": "Air China",
        "departure_airport": "PEK",
        "arrival_airport": "SHA",
        "price": 680.00,
        "added_at": "2024-01-20T10:30:00Z"
      }
    ],
    "total": 5
  }
}
```

---

### 4. 添加收藏
**POST** `/users/{user_id}/favorites`

**权限**: 需要认证

**请求体**:
```json
{
  "flight_number": "CA123"
}
```

**响应**:
```json
{
  "code": 0,
  "message": "Flight added to favorites",
  "data": {
    "favorite_id": "fav_123"
  }
}
```

---

### 5. 移除收藏
**DELETE** `/users/{user_id}/favorites/{flight_number}`

**权限**: 需要认证

**响应**:
```json
{
  "code": 0,
  "message": "Flight removed from favorites"
}
```

---

## 评价相关 API

### 1. 提交评价
**POST** `/bookings/{booking_id}/review`

**权限**: 需要认证

**请求体**:
```json
{
  "rating": 4.5,
  "title": "Great flight experience",
  "content": "The flight was on time and the service was excellent.",
  "categories": {
    "service": 5,
    "cleanliness": 4,
    "food_quality": 4,
    "comfort": 4.5
  }
}
```

**响应**:
```json
{
  "code": 0,
  "message": "Review submitted successfully",
  "data": {
    "review_id": "review_123",
    "booking_id": "BK20240120001",
    "created_at": "2024-01-20T10:30:00Z"
  }
}
```

---

## 支付相关 API

### 1. 验证支付方式
**POST** `/payments/validate`

**权限**: 需要认证

**请求体**:
```json
{
  "payment_method": "credit_card",
  "card_number": "4111111111111111",
  "cardholder_name": "John Doe",
  "expiry_month": "12",
  "expiry_year": "2025",
  "cvv": "123"
}
```

**响应**:
```json
{
  "code": 0,
  "message": "Payment method validated",
  "data": {
    "valid": true,
    "card_type": "Visa",
    "last_four": "1111"
  }
}
```

---

### 2. 处理支付
**POST** `/payments/process`

**权限**: 需要认证

**请求体**:
```json
{
  "booking_id": "BK20240120001",
  "amount": 840.00,
  "currency": "CNY",
  "payment_method": "credit_card",
  "card_number": "4111111111111111",
  "cardholder_name": "John Doe",
  "expiry_month": "12",
  "expiry_year": "2025",
  "cvv": "123"
}
```

**响应**:
```json
{
  "code": 0,
  "message": "Payment processed successfully",
  "data": {
    "transaction_id": "txn_123456789",
    "booking_id": "BK20240120001",
    "amount": 840.00,
    "currency": "CNY",
    "status": "success",
    "payment_method": "credit_card",
    "last_four": "1111",
    "processed_at": "2024-01-20T10:30:00Z"
  }
}
```

---

## 系统相关 API

### 1. 获取系统状态
**GET** `/system/status`

**权限**: 无需认证

**响应**:
```json
{
  "code": 0,
  "message": "System status retrieved",
  "data": {
    "status": "operational",
    "uptime_hours": 720.5,
    "response_time_ms": 45,
    "active_users": 1250,
    "total_bookings_today": 350,
    "alerts": []
  }
}
```

---

### 2. 获取统计数据
**GET** `/statistics/flights`

**权限**: 无需认证

**响应**:
```json
{
  "code": 0,
  "message": "Statistics retrieved",
  "data": {
    "total_flights_today": 450,
    "total_passengers_today": 60000,
    "on_time_percentage": 96.5,
    "cancelled_flights": 5,
    "delayed_flights": 12,
    "top_routes": [
      {
        "route": "PEK-SHA",
        "flights": 45,
        "passengers": 5400
      }
    ]
  }
}
```

---

### 3. 获取通知
**GET** `/users/{user_id}/notifications`

**权限**: 需要认证

**查询参数**:
- `unread_only`: 仅未读 (默认false)

**响应**:
```json
{
  "code": 0,
  "message": "Notifications retrieved",
  "data": {
    "notifications": [
      {
        "notification_id": "notif_123",
        "type": "booking_confirmed",
        "title": "预订确认",
        "message": "您的航班BK20240120001已确认",
        "read": false,
        "created_at": "2024-01-20T10:30:00Z",
        "data": {
          "booking_id": "BK20240120001"
        }
      }
    ],
    "unread_count": 3
  }
}
```

---

### 4. 标记通知已读
**POST** `/notifications/{notification_id}/read`

**权限**: 需要认证

**响应**:
```json
{
  "code": 0,
  "message": "Notification marked as read"
}
```

---

## 错误代码参考

| 代码 | 说明 | 处理方法 |
|------|------|---------|
| 4001 | Unauthorized | 重新登录 |
| 4002 | Resource not found | 检查请求参数 |
| 4003 | Forbidden | 权限不足 |
| 4004 | Validation error | 检查输入数据 |
| 5000 | Server error | 重试或联系支持 |
| 5001 | Database error | 重试或联系支持 |
| 5002 | Payment gateway error | 使用其他支付方式 |

---

## 最佳实践

1. **认证管理**
   - 登录后存储 token 和 refresh_token
   - Token 过期时使用 refresh_token 刷新
   - 注销时清除所有令牌

2. **错误处理**
   - 检查响应状态码
   - 实现重试机制（指数退避）
   - 显示用户友好的错误消息

3. **性能优化**
   - 使用缓存减少 API 调用
   - 实现分页处理大数据集
   - 批量操作时使用事务

4. **安全性**
   - 不在客户端存储敏感信息
   - 验证 SSL/TLS 证书
   - 使用安全的支付方式

---

## 示例

### 完整的登录流程

```javascript
// 1. 登录
POST /auth/login
{
  "username": "user@example.com",
  "password": "password123"
}

// 2. 获取 token 和 refresh_token
// token 有效期 1 小时，refresh_token 有效期 7 天

// 3. 在后续请求中使用 token
GET /users/user_123
Authorization: Bearer eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9...

// 4. Token 过期时刷新
POST /auth/refresh
Authorization: Bearer refresh_token_here

// 5. 获取新的 token

// 6. 使用新 token 继续操作
```

### 完整的航班搜索和预订流程

```javascript
// 1. 搜索航班
POST /flights/search
{
  "departure": "PEK",
  "destination": "SHA",
  "date": "2024-02-15",
  "passengers": 2,
  "seat_class": "economy"
}

// 2. 获取返回的 search_id 和可用航班

// 3. 选择航班后获取详情
GET /flights/CA123

// 4. 查看可用座位
GET /flights/CA123/seats

// 5. 创建预订
POST /bookings
{
  "flight_number": "CA123",
  "search_id": "search_123456",
  "passengers": [...],
  "seats": ["1A", "1B"]
}

// 6. 处理支付
POST /payments/process
{
  "booking_id": "BK20240120001",
  "amount": 840.00,
  ...
}

// 7. 获取预订确认
GET /bookings/BK20240120001
```

---

## 版本历史

| 版本 | 发布日期 | 变更 |
|------|---------|------|
| 1.0 | 2024-01-20 | 初始版本，包含基础 API |
| 1.1 | 计划中 | 添加更多支付方式 |
| 2.0 | 计划中 | 重大重构，支持更多功能 |

