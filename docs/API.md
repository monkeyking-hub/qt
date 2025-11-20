# Flight Management System API 文档

## 概述

本文档描述了Flight Management System的API接口，包括航班管理、用户管理、预订管理等核心功能的接口规范。

## 基础信息

- **基础URL**: `https://api.flightsystem.com/v1`
- **认证方式**: Bearer Token
- **数据格式**: JSON
- **字符编码**: UTF-8

## 通用响应格式

### 成功响应
```json
{
    "success": true,
    "data": {
        // 具体数据
    },
    "message": "操作成功",
    "timestamp": "2024-01-01T12:00:00Z"
}
```

### 错误响应
```json
{
    "success": false,
    "error": {
        "code": "ERROR_CODE",
        "message": "错误描述",
        "details": "详细错误信息"
    },
    "timestamp": "2024-01-01T12:00:00Z"
}
```

## 航班管理 API

### 1. 搜索航班

**接口**: `GET /flights/search`

**描述**: 根据条件搜索航班

**请求参数**:
```json
{
    "departure": "北京",        // 出发地 (必需)
    "destination": "上海",       // 目的地 (必需)
    "date": "2024-01-15",      // 出发日期 (必需)
    "trip_type": "oneway",     // 航班类型: oneway, roundtrip, multicity
    "passengers": 1,            // 乘客人数
    "class": "economy"          // 舱位等级: economy, business, first
}
```

**响应示例**:
```json
{
    "success": true,
    "data": {
        "flights": [
            {
                "flight_number": "CA1234",
                "airline": "中国国际航空",
                "departure": "北京首都",
                "destination": "上海浦东",
                "departure_time": "08:00",
                "arrival_time": "10:30",
                "price": 1280.00,
                "currency": "CNY",
                "status": "on_time",
                "aircraft": "Boeing 737-800",
                "gate": "A12",
                "terminal": "T3"
            }
        ],
        "total": 1,
        "page": 1,
        "per_page": 20
    }
}
```

### 2. 获取航班详情

**接口**: `GET /flights/{flight_number}`

**描述**: 获取指定航班的详细信息

**路径参数**:
- `flight_number`: 航班号

**响应示例**:
```json
{
    "success": true,
    "data": {
        "flight_number": "CA1234",
        "airline": "中国国际航空",
        "departure": {
            "airport": "北京首都国际机场",
            "code": "PEK",
            "terminal": "T3",
            "gate": "A12",
            "time": "2024-01-15T08:00:00Z"
        },
        "arrival": {
            "airport": "上海浦东国际机场",
            "code": "PVG",
            "terminal": "T2",
            "gate": "B08",
            "time": "2024-01-15T10:30:00Z"
        },
        "aircraft": {
            "type": "Boeing 737-800",
            "registration": "B-1234",
            "capacity": 189
        },
        "status": "on_time",
        "duration": "2h 30m",
        "distance": 1075,
        "prices": {
            "economy": 1280.00,
            "business": 3840.00,
            "first": 6400.00
        }
    }
}
```

### 3. 更新航班状态

**接口**: `PUT /flights/{flight_number}/status`

**描述**: 更新航班状态

**请求体**:
```json
{
    "status": "delayed",
    "delay_minutes": 15,
    "reason": "天气原因",
    "new_departure_time": "08:15",
    "new_arrival_time": "10:45"
}
```

## 用户管理 API

### 1. 用户登录

**接口**: `POST /auth/login`

**描述**: 用户身份验证

**请求体**:
```json
{
    "username": "user123",
    "password": "password123",
    "remember_me": true
}
```

**响应示例**:
```json
{
    "success": true,
    "data": {
        "user": {
            "id": 1001,
            "username": "user123",
            "email": "user@example.com",
            "role": "user",
            "status": "active"
        },
        "token": "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9...",
        "expires_in": 3600
    }
}
```

### 2. 用户注册

**接口**: `POST /auth/register`

**描述**: 创建新用户账户

**请求体**:
```json
{
    "username": "newuser",
    "password": "password123",
    "email": "newuser@example.com",
    "phone": "+86-13800138000",
    "first_name": "张",
    "last_name": "三",
    "birth_date": "1990-01-01",
    "gender": "male"
}
```

### 3. 获取用户信息

**接口**: `GET /users/{user_id}`

**描述**: 获取用户详细信息

**响应示例**:
```json
{
    "success": true,
    "data": {
        "id": 1001,
        "username": "user123",
        "email": "user@example.com",
        "phone": "+86-13800138000",
        "first_name": "张",
        "last_name": "三",
        "role": "user",
        "status": "active",
        "created_at": "2024-01-01T12:00:00Z",
        "last_login": "2024-01-15T08:30:00Z"
    }
}
```

### 4. 更新用户信息

**接口**: `PUT /users/{user_id}`

**描述**: 更新用户信息

**请求体**:
```json
{
    "email": "updated@example.com",
    "phone": "+86-13900139000",
    "first_name": "李",
    "last_name": "四"
}
```

## 预订管理 API

### 1. 创建预订

**接口**: `POST /bookings`

**描述**: 创建新的航班预订

**请求体**:
```json
{
    "flight_number": "CA1234",
    "user_id": 1001,
    "passengers": [
        {
            "first_name": "张",
            "last_name": "三",
            "id_number": "110101199001011234",
            "birth_date": "1990-01-01",
            "gender": "male",
            "class": "economy"
        }
    ],
    "services": {
        "insurance": true,
        "special_meal": false,
        "preferred_seat": true
    },
    "payment_method": "credit_card",
    "contact": {
        "phone": "+86-13800138000",
        "email": "user@example.com"
    }
}
```

**响应示例**:
```json
{
    "success": true,
    "data": {
        "booking_id": "BK20240115001",
        "flight_number": "CA1234",
        "status": "confirmed",
        "total_price": 1430.00,
        "currency": "CNY",
        "passengers": 1,
        "booking_date": "2024-01-15T09:00:00Z",
        "confirmation_code": "ABC123"
    }
}
```

### 2. 获取预订详情

**接口**: `GET /bookings/{booking_id}`

**描述**: 获取预订详细信息

**响应示例**:
```json
{
    "success": true,
    "data": {
        "booking_id": "BK20240115001",
        "flight": {
            "flight_number": "CA1234",
            "airline": "中国国际航空",
            "departure": "北京首都",
            "destination": "上海浦东",
            "departure_time": "2024-01-15T08:00:00Z",
            "arrival_time": "2024-01-15T10:30:00Z"
        },
        "passengers": [
            {
                "name": "张三",
                "id_number": "110101199001011234",
                "seat": "12A",
                "class": "economy"
            }
        ],
        "status": "confirmed",
        "total_price": 1430.00,
        "services": {
            "insurance": true,
            "special_meal": false,
            "preferred_seat": true
        }
    }
}
```

### 3. 获取用户预订列表

**接口**: `GET /users/{user_id}/bookings`

**描述**: 获取指定用户的所有预订

**查询参数**:
- `status`: 预订状态过滤 (confirmed, cancelled, completed)
- `limit`: 返回数量限制
- `offset`: 偏移量

### 4. 取消预订

**接口**: `PUT /bookings/{booking_id}/cancel`

**描述**: 取消预订

**请求体**:
```json
{
    "reason": "行程变更",
    "refund_method": "original"
}
```

## 统计信息 API

### 1. 航班统计

**接口**: `GET /statistics/flights`

**描述**: 获取航班统计数据

**查询参数**:
- `date_from`: 开始日期
- `date_to`: 结束日期
- `airline`: 航空公司过滤

**响应示例**:
```json
{
    "success": true,
    "data": {
        "total_flights": 1250,
        "on_time_flights": 1125,
        "delayed_flights": 100,
        "cancelled_flights": 25,
        "on_time_rate": 90.0,
        "popular_routes": [
            {
                "route": "北京-上海",
                "flight_count": 350
            }
        ]
    }
}
```

### 2. 用户统计

**接口**: `GET /statistics/users`

**描述**: 获取用户统计数据

### 3. 预订统计

**接口**: `GET /statistics/bookings`

**描述**: 获取预订统计数据

## 系统管理 API

### 1. 系统状态

**接口**: `GET /system/status`

**描述**: 获取系统运行状态

**响应示例**:
```json
{
    "success": true,
    "data": {
        "status": "healthy",
        "version": "1.0.0",
        "uptime": 86400,
        "database": "connected",
        "cache": "connected",
        "api_response_time": 120
    }
}
```

### 2. 系统配置

**接口**: `GET /system/config`

**描述**: 获取系统配置信息

## 错误代码

| 错误代码 | HTTP状态码 | 描述 |
|---------|-----------|------|
| INVALID_REQUEST | 400 | 请求参数无效 |
| UNAUTHORIZED | 401 | 未授权访问 |
| FORBIDDEN | 403 | 禁止访问 |
| NOT_FOUND | 404 | 资源不存在 |
| METHOD_NOT_ALLOWED | 405 | 方法不允许 |
| CONFLICT | 409 | 资源冲突 |
| RATE_LIMIT_EXCEEDED | 429 | 请求频率超限 |
| INTERNAL_ERROR | 500 | 服务器内部错误 |
| SERVICE_UNAVAILABLE | 503 | 服务不可用 |

## 限流规则

- 每个IP每分钟最多100个请求
- 每个用户每分钟最多50个请求
- 搜索接口每分钟最多20个请求

## SDK和示例代码

### C++/Qt 示例

```cpp
#include "apimanager.h"

// 搜索航班
APIManager apiManager;
connect(&apiManager, &APIManager::flightSearchCompleted, 
        [](const QJsonArray &flights) {
    // 处理搜索结果
});

apiManager.searchFlights("北京", "上海", QDate(2024, 1, 15));
```

### JavaScript 示例

```javascript
// 搜索航班
fetch('/api/v1/flights/search', {
    method: 'POST',
    headers: {
        'Content-Type': 'application/json',
        'Authorization': 'Bearer ' + token
    },
    body: JSON.stringify({
        departure: '北京',
        destination: '上海',
        date: '2024-01-15'
    })
})
.then(response => response.json())
.then(data => console.log(data));
```

## 更新日志

### v1.0.0 (2024-01-01)
- 初始API版本发布
- 实现基础航班管理功能
- 添加用户认证和授权
- 实现预订管理功能

---

如有任何问题或建议，请联系API支持团队: api-support@flightsystems.com