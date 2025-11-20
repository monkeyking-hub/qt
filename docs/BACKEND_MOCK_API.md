# 后端 Mock API 实现指南

本指南提供了使用流行框架实现Mock API服务的示例，用于测试前端应用。

## 方式1: 使用 Node.js + Express

### 项目初始化

```bash
mkdir flight-api-mock
cd flight-api-mock
npm init -y
npm install express cors body-parser jsonwebtoken bcryptjs dotenv
npm install --save-dev nodemon
```

### 目录结构

```
flight-api-mock/
├── server.js
├── .env
├── routes/
│   ├── auth.js
│   ├── flights.js
│   ├── bookings.js
│   └── users.js
├── middleware/
│   └── auth.js
├── models/
│   ├── user.js
│   ├── flight.js
│   └── booking.js
└── data/
    ├── users.json
    ├── flights.json
    └── bookings.json
```

### 基础服务器 (server.js)

```javascript
const express = require('express');
const cors = require('cors');
const bodyParser = require('body-parser');
require('dotenv').config();

const app = express();

// 中间件
app.use(cors());
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: true }));

// 错误处理
app.use((err, req, res, next) => {
    console.error(err.stack);
    res.status(500).json({
        code: 5000,
        message: 'Server error',
        error: err.message
    });
});

// 路由
const authRoutes = require('./routes/auth');
const flightRoutes = require('./routes/flights');
const bookingRoutes = require('./routes/bookings');
const userRoutes = require('./routes/users');

app.use('/api/v1/auth', authRoutes);
app.use('/api/v1/flights', flightRoutes);
app.use('/api/v1/bookings', bookingRoutes);
app.use('/api/v1/users', userRoutes);

// 启动服务器
const PORT = process.env.PORT || 3000;
app.listen(PORT, () => {
    console.log(`API Mock Server running on http://localhost:${PORT}`);
});
```

### 认证路由 (routes/auth.js)

```javascript
const express = require('express');
const jwt = require('jsonwebtoken');
const bcrypt = require('bcryptjs');
const fs = require('fs');
const path = require('path');

const router = express.Router();
const usersFile = path.join(__dirname, '../data/users.json');

// 读取用户数据
function readUsers() {
    const data = fs.readFileSync(usersFile, 'utf8');
    return JSON.parse(data);
}

// 写入用户数据
function writeUsers(users) {
    fs.writeFileSync(usersFile, JSON.stringify(users, null, 2));
}

// 生成Token
function generateToken(userId) {
    return jwt.sign(
        { userId },
        process.env.JWT_SECRET || 'secret-key',
        { expiresIn: '1h' }
    );
}

// 登录
router.post('/login', async (req, res) => {
    const { username, password } = req.body;
    
    if (!username || !password) {
        return res.status(400).json({
            code: 4000,
            message: 'Username and password are required'
        });
    }
    
    const users = readUsers();
    const user = users.find(u => u.username === username || u.email === username);
    
    if (!user) {
        return res.status(401).json({
            code: 4001,
            message: 'Invalid username or password'
        });
    }
    
    // 简化: 直接比较密码 (实际应用中应使用bcrypt)
    if (user.password !== password) {
        return res.status(401).json({
            code: 4001,
            message: 'Invalid username or password'
        });
    }
    
    const token = generateToken(user.id);
    const refreshToken = jwt.sign(
        { userId: user.id },
        process.env.JWT_REFRESH_SECRET || 'refresh-secret',
        { expiresIn: '7d' }
    );
    
    res.json({
        code: 0,
        message: 'Login successful',
        data: {
            user_id: user.id,
            token,
            refresh_token: refreshToken,
            expires_in: 3600,
            user: {
                user_id: user.id,
                username: user.username,
                email: user.email,
                full_name: user.full_name,
                avatar_url: user.avatar_url,
                role: user.role
            }
        }
    });
});

// 注册
router.post('/register', (req, res) => {
    const { email, username, password, full_name, phone } = req.body;
    
    // 验证输入
    if (!email || !username || !password || !full_name) {
        return res.status(400).json({
            code: 4000,
            message: 'Missing required fields'
        });
    }
    
    const users = readUsers();
    
    // 检查用户名或邮箱是否已存在
    if (users.some(u => u.username === username)) {
        return res.status(400).json({
            code: 4000,
            message: 'Username already exists',
            errors: [{ field: 'username', message: 'This username is taken' }]
        });
    }
    
    if (users.some(u => u.email === email)) {
        return res.status(400).json({
            code: 4000,
            message: 'Email already exists',
            errors: [{ field: 'email', message: 'This email is already registered' }]
        });
    }
    
    // 创建新用户
    const newUser = {
        id: 'user_' + Date.now(),
        username,
        email,
        password, // 实际应用中应进行加密
        full_name,
        phone,
        role: 'customer',
        status: 'active',
        created_at: new Date().toISOString()
    };
    
    users.push(newUser);
    writeUsers(users);
    
    res.json({
        code: 0,
        message: 'Registration successful',
        data: {
            user_id: newUser.id,
            email: newUser.email,
            username: newUser.username,
            message: 'Welcome! Please verify your email.'
        }
    });
});

// 刷新Token
router.post('/refresh', (req, res) => {
    const token = req.headers.authorization?.split(' ')[1];
    
    if (!token) {
        return res.status(401).json({
            code: 4001,
            message: 'Token not provided'
        });
    }
    
    try {
        jwt.verify(token, process.env.JWT_REFRESH_SECRET || 'refresh-secret');
    } catch (err) {
        return res.status(401).json({
            code: 4001,
            message: 'Invalid refresh token'
        });
    }
    
    const decoded = jwt.decode(token);
    const newToken = generateToken(decoded.userId);
    
    res.json({
        code: 0,
        message: 'Token refreshed',
        data: {
            token: newToken,
            expires_in: 3600
        }
    });
});

module.exports = router;
```

### 航班路由 (routes/flights.js)

```javascript
const express = require('express');
const fs = require('fs');
const path = require('path');

const router = express.Router();
const flightsFile = path.join(__dirname, '../data/flights.json');

// 读取航班数据
function readFlights() {
    const data = fs.readFileSync(flightsFile, 'utf8');
    return JSON.parse(data);
}

// 搜索航班
router.post('/search', (req, res) => {
    const { departure, destination, date, return_date, passengers, seat_class } = req.body;
    
    if (!departure || !destination || !date) {
        return res.status(400).json({
            code: 4000,
            message: 'Missing required parameters'
        });
    }
    
    const flights = readFlights();
    
    // 简单的筛选逻辑
    const filtered = flights.filter(f => 
        f.departure_airport === departure &&
        f.arrival_airport === destination &&
        f.departure_time.startsWith(date)
    );
    
    res.json({
        code: 0,
        message: 'Search successful',
        data: {
            flights: filtered,
            search_id: 'search_' + Date.now(),
            total_flights: filtered.length
        }
    });
});

// 获取航班详情
router.get('/:flightNumber', (req, res) => {
    const { flightNumber } = req.params;
    const flights = readFlights();
    const flight = flights.find(f => f.flight_number === flightNumber);
    
    if (!flight) {
        return res.status(404).json({
            code: 4002,
            message: 'Flight not found'
        });
    }
    
    res.json({
        code: 0,
        message: 'Flight details retrieved',
        data: flight
    });
});

// 获取可用座位
router.get('/:flightNumber/seats', (req, res) => {
    const { flightNumber } = req.params;
    const flights = readFlights();
    const flight = flights.find(f => f.flight_number === flightNumber);
    
    if (!flight) {
        return res.status(404).json({
            code: 4002,
            message: 'Flight not found'
        });
    }
    
    // 生成座位列表
    const seats = [];
    const columns = ['A', 'B', 'C', 'D', 'E', 'F'];
    const rows = 50;
    
    for (let row = 1; row <= rows; row++) {
        for (let col = 0; col < columns.length; col++) {
            seats.push({
                seat_id: `${row}${columns[col]}`,
                seat_number: `${row}${columns[col]}`,
                row,
                column: columns[col],
                class: row <= 6 ? 'first' : row <= 15 ? 'business' : 'economy',
                status: Math.random() > 0.8 ? 'booked' : 'available',
                price_adjustment: 0,
                is_emergency_exit: row === 25,
                is_window: col === 0 || col === 5
            });
        }
    }
    
    res.json({
        code: 0,
        message: 'Seats retrieved',
        data: {
            seats,
            total_seats: seats.length,
            available_seats: seats.filter(s => s.status === 'available').length
        }
    });
});

// 获取评价
router.get('/:flightNumber/reviews', (req, res) => {
    const page = req.query.page || 1;
    const limit = req.query.limit || 10;
    
    const reviews = [
        {
            review_id: 'review_1',
            user_id: 'user_1',
            username: 'john_doe',
            avatar_url: 'https://...',
            rating: 4.5,
            title: 'Great flight experience',
            content: 'The flight was on time and the service was excellent.',
            verified_purchase: true,
            helpful_count: 23,
            created_at: new Date().toISOString()
        }
    ];
    
    res.json({
        code: 0,
        message: 'Reviews retrieved',
        data: {
            reviews,
            average_rating: 4.5,
            total_reviews: 230,
            page: parseInt(page),
            pages: Math.ceil(230 / limit)
        }
    });
});

module.exports = router;
```

### 示例数据文件 (data/flights.json)

```json
[
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
]
```

---

## 方式2: 使用 Python + Flask

### 项目初始化

```bash
mkdir flight-api-mock-python
cd flight-api-mock-python
python -m venv venv
source venv/bin/activate  # Windows: venv\Scripts\activate
pip install flask flask-cors flask-jwt-extended pyyaml
```

### 基础应用 (app.py)

```python
from flask import Flask, request, jsonify
from flask_cors import CORS
from flask_jwt_extended import JWTManager, create_access_token, jwt_required
from datetime import datetime, timedelta
import json
import os

app = Flask(__name__)
CORS(app)

# JWT配置
app.config['JWT_SECRET_KEY'] = os.environ.get('JWT_SECRET', 'secret-key')
jwt = JWTManager(app)

# 数据存储
users_db = []
flights_db = []
bookings_db = []

# 初始化示例数据
def init_data():
    global flights_db
    flights_db = [
        {
            "flight_number": "CA123",
            "airline": "Air China",
            "departure_airport": "PEK",
            "arrival_airport": "SHA",
            "departure_time": "2024-02-15T10:00:00Z",
            "arrival_time": "2024-02-15T12:30:00Z",
            "duration_minutes": 150,
            "price": {
                "economy": 680.00,
                "business": 1680.00
            }
        }
    ]

# 登录路由
@app.route('/api/v1/auth/login', methods=['POST'])
def login():
    data = request.get_json()
    username = data.get('username')
    password = data.get('password')
    
    if not username or not password:
        return jsonify({
            "code": 4000,
            "message": "Missing credentials"
        }), 400
    
    # 简化: 任何有效的用户名/密码都返回成功
    user_id = f"user_{hash(username) % 10000}"
    token = create_access_token(identity=user_id, expires_delta=timedelta(hours=1))
    
    return jsonify({
        "code": 0,
        "message": "Login successful",
        "data": {
            "user_id": user_id,
            "token": token,
            "expires_in": 3600
        }
    })

# 航班搜索
@app.route('/api/v1/flights/search', methods=['POST'])
def search_flights():
    data = request.get_json()
    
    return jsonify({
        "code": 0,
        "message": "Search successful",
        "data": {
            "flights": flights_db,
            "search_id": f"search_{int(datetime.now().timestamp())}",
            "total_flights": len(flights_db)
        }
    })

# 保护的路由示例
@app.route('/api/v1/users/<user_id>', methods=['GET'])
@jwt_required()
def get_user(user_id):
    return jsonify({
        "code": 0,
        "message": "User profile retrieved",
        "data": {
            "user_id": user_id,
            "username": "john_doe",
            "email": "john@example.com",
            "full_name": "John Doe"
        }
    })

# 错误处理
@app.errorhandler(404)
def not_found(error):
    return jsonify({
        "code": 4002,
        "message": "Resource not found"
    }), 404

if __name__ == '__main__':
    init_data()
    app.run(debug=True, port=3000)
```

---

## 方式3: 使用 Go + Gin

### 项目初始化

```bash
mkdir flight-api-mock-go
cd flight-api-mock-go
go mod init flight-api-mock
go get github.com/gin-gonic/gin
go get github.com/golang-jwt/jwt/v5
```

### 基础服务器 (main.go)

```go
package main

import (
	"github.com/gin-gonic/gin"
	"net/http"
	"time"
)

type Flight struct {
	FlightNumber  string    `json:"flight_number"`
	Airline       string    `json:"airline"`
	Departure     string    `json:"departure_airport"`
	Arrival       string    `json:"arrival_airport"`
	DepartureTime time.Time `json:"departure_time"`
	Price         map[string]float64 `json:"price"`
}

type LoginRequest struct {
	Username string `json:"username" binding:"required"`
	Password string `json:"password" binding:"required"`
}

type Response struct {
	Code    int         `json:"code"`
	Message string      `json:"message"`
	Data    interface{} `json:"data"`
}

func main() {
	r := gin.Default()
	
	// CORS中间件
	r.Use(func(c *gin.Context) {
		c.Writer.Header().Set("Access-Control-Allow-Origin", "*")
		c.Writer.Header().Set("Access-Control-Allow-Credentials", "true")
		c.Writer.Header().Set("Access-Control-Allow-Headers", "Content-Type, Content-Length, Accept-Encoding, X-CSRF-Token, Authorization, accept, origin, Cache-Control, X-Requested-With")
		c.Writer.Header().Set("Access-Control-Allow-Methods", "POST, OPTIONS, GET, PUT, DELETE")
		
		if c.Request.Method == "OPTIONS" {
			c.AbortWithStatus(204)
			return
		}
		c.Next()
	})
	
	// 登录路由
	r.POST("/api/v1/auth/login", func(c *gin.Context) {
		var loginReq LoginRequest
		if err := c.ShouldBindJSON(&loginReq); err != nil {
			c.JSON(http.StatusBadRequest, Response{
				Code:    4000,
				Message: "Invalid request",
			})
			return
		}
		
		c.JSON(http.StatusOK, Response{
			Code:    0,
			Message: "Login successful",
			Data: map[string]interface{}{
				"user_id": "user_123",
				"token":   "jwt_token_here",
				"expires_in": 3600,
			},
		})
	})
	
	// 航班搜索
	r.POST("/api/v1/flights/search", func(c *gin.Context) {
		flights := []Flight{
			{
				FlightNumber:  "CA123",
				Airline:       "Air China",
				Departure:     "PEK",
				Arrival:       "SHA",
				DepartureTime: time.Now(),
				Price: map[string]float64{
					"economy":  680.00,
					"business": 1680.00,
				},
			},
		}
		
		c.JSON(http.StatusOK, Response{
			Code:    0,
			Message: "Search successful",
			Data: map[string]interface{}{
				"flights":      flights,
				"search_id":    "search_123456",
				"total_flights": len(flights),
			},
		})
	})
	
	r.Run(":3000")
}
```

---

## 测试 Mock API

### 使用 cURL

```bash
# 登录
curl -X POST http://localhost:3000/api/v1/auth/login \
  -H "Content-Type: application/json" \
  -d '{"username":"user@example.com","password":"password123"}'

# 搜索航班
curl -X POST http://localhost:3000/api/v1/flights/search \
  -H "Content-Type: application/json" \
  -d '{
    "departure": "PEK",
    "destination": "SHA",
    "date": "2024-02-15",
    "passengers": 1,
    "seat_class": "economy"
  }'
```

### 使用 Postman

1. 导入 API 集合
2. 设置环境变量 (base_url, token)
3. 运行请求

---

## 配置前端连接 Mock API

在 Qt 应用中:

```cpp
APIManager apiManager;
apiManager.setBaseUrl("http://localhost:3000/api/v1");
```

---

## 生产部署建议

### Docker 部署

```dockerfile
FROM node:16
WORKDIR /app
COPY . .
RUN npm install
EXPOSE 3000
CMD ["npm", "start"]
```

构建和运行:
```bash
docker build -t flight-api-mock .
docker run -p 3000:3000 flight-api-mock
```

### 使用 Docker Compose

```yaml
version: '3'
services:
  api:
    build: .
    ports:
      - "3000:3000"
    environment:
      - JWT_SECRET=your-secret-key
      - NODE_ENV=production
```

---

## 注意事项

1. **安全性**: 生产环境中必须使用真正的密码加密和JWT实现
2. **数据持久化**: 考虑使用数据库 (MongoDB, PostgreSQL等)
3. **错误处理**: 完善的错误处理和日志记录
4. **性能**: 实现缓存、分页和索引优化
5. **版本控制**: API 版本管理
6. **文档**: 使用 Swagger/OpenAPI 生成API文档

