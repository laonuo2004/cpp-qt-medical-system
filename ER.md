ER图

页面
| 模块     | 字段                                           | 数据类型 / 约束        |
|----------|------------------------------------------------|------------------------|
| 患者信息 | 姓名                                           | 字符串                 |
| 患者信息 | 身份证号                                       | 18位数字               |
| 患者信息 | 性别                                           | 选项：男 / 女          |
| 患者信息 | 出生年月                                       | 年月日                 |
| 患者信息 | 年龄                                           | 自动识别               |
| 患者信息 | 联系方式（手机号）                             | 11位数字               |
| 患者信息 | 家庭住址                                       | 字符串                 |
| 患者信息 | 紧急联系人（家属、监护人）姓名                 | 字符串                 |
| 患者信息 | 紧急联系人（家属、监护人）的联系方式（手机号） | 11位数字               |


| 模块     | 字段               | 数据类型 / 约束   |
|----------|--------------------|-------------------|
| 医生信息 | 工号               | 字符串
| 医生信息 | 姓名               | 字符串            |
| 医生信息 | 性别               | 选项：男 / 女     |
| 医生信息 | 年龄               | 计算              |
| 医生信息 | 联系方式（手机号） | 11位数字          |
| 医生信息 | 所在科室           | 字符串            |
| 医生信息 | 医生职务           | 字符串            |


| 模块（流程）     | 字段                     | 数据类型 / 约束 |
|------------------|--------------------------|------------------|
| 挂号（患者操作） | 患者姓名                 | 自动匹配         |
| 挂号（患者操作） | 患者年龄                 | 自动匹配         |
| 挂号（患者操作） | 医生科室（先）           | 选择             |
| 挂号（患者操作） | 可选医生（在所在科室选） | 选择             |
| 挂号（患者操作） | 预约的时间段             | 选择             |


```mermaid
erDiagram
    USERS {
        int user_id PK "用户ID (主键)"
        string username "用户名 (唯一)"
        string email "电子邮箱 (唯一)"
        string password_hash "哈希密码"
        string role "角色 (patient, doctor, admin)"
        datetime created_at "账户创建时间"
    }

    PATIENTS {
        int patient_id PK "患者ID (主键)"
        int user_id FK "关联用户ID (一对一)"
        string full_name "真实姓名"
        string id_card_no "身份证号 (唯一)"
        date birth_date "出生日期"
        string phone_no "手机号 (唯一)"
    }

    DOCTORS {
        int doctor_id PK "医生ID (主键)"
        int user_id FK "关联用户ID (一对一)"
        string full_name "真实姓名"
        string department "科室"
        string title "职称"
        string photo_url "照片URL"
    }

    APPOINTMENTS {
        int appointment_id PK "预约ID (主键)"
        int patient_id FK "患者ID (外键)"
        int doctor_id FK "医生ID (外键)"
        datetime appointment_time "预约时间"
        string status "状态 (scheduled, completed, cancelled)"
        string payment_status "支付状态 (unpaid, paid)"
    }

    MEDICAL_RECORDS {
        int record_id PK "病历ID (主键)"
        int appointment_id FK "关联预约ID (唯一)"
        text diagnosis_notes "诊断记录"
        datetime created_at "创建时间"
    }

    PRESCRIPTIONS {
        int prescription_id PK "处方ID (主键)"
        int record_id FK "关联病历ID (外键)"
        text details "处方详情"
        datetime issued_at "开具时间"
    }

    CHAT_MESSAGES {
        int message_id PK "消息ID (主键)"
        int sender_id FK "发送者ID (外键)"
        int receiver_id FK "接收者ID (外键)"
        text content "消息内容"
        datetime sent_at "发送时间"
    }

    CASE_TEMPLATES {
        int template_id PK "模板ID (主键)"
        int created_by_doctor_id FK "创建医生ID (外键)"
        string template_name "模板名称"
        text default_diagnosis "默认诊断"
        text default_prescription "默认处方"
    }

    ATTENDANCE {
        int attendance_id PK "考勤ID (主键)"
        int doctor_id FK "医生ID (外键)"
        date date "日期"
        time check_in_time "签到时间"
        time check_out_time "签退时间"
    }

    LEAVE_REQUESTS {
        int request_id PK "申请ID (主键)"
        int doctor_id FK "医生ID (外键)"
        date start_date "开始日期"
        date end_date "结束日期"
        text reason "请假事由"
        string status "状态 (pending, approved, rejected)"
    }

    USERS ||--o| PATIENTS : "拥有个人资料"
    USERS ||--o| DOCTORS : "拥有个人资料"
    USERS }o--o{ CHAT_MESSAGES : "发送"
    USERS }o--o{ CHAT_MESSAGES : "接收"
    
    PATIENTS ||--|{ APPOINTMENTS : "发起预约"
    DOCTORS ||--|{ APPOINTMENTS : "接受预约"
    DOCTORS ||--|{ CASE_TEMPLATES : "创建模板"
    DOCTORS ||--|{ ATTENDANCE : "记录考勤"
    DOCTORS ||--|{ LEAVE_REQUESTS : "发起申请"

    APPOINTMENTS ||--|| MEDICAL_RECORDS : "生成"
    MEDICAL_RECORDS ||--|{ PRESCRIPTIONS : "包含"
```

数据结构---用户
```mermaid
erDiagram
    Patient {
        VARCHAR50  LoginID PK
        CHAR18     ID
        VARCHAR50  Name
        CHAR1      Sex
        DATE       BirthDate
        TINYINT    Age
        CHAR11     PhoneNumber
        VARCHAR50  Address
        VARCHAR50  SOS
        CHAR11     SOSPhone
        VARCHAR50  Passward
    }

    Doctor {
        VARCHAR50  ID  PK
        VARCHAR50  LoginID 
        VARCHAR50  Position
        VARCHAR50  Department
        VARCHAR50  Name
        CHAR1      Sex
        TINYINT    Age
        CHAR11     PhoneNumber
        VARCHAR50  Passward
    }

    Admin {
        VARCHAR50  LoginID PK
        VARCHAR50  Passward
        CHAR11     PhoneNumber
    }

    Appointment预约 {
        VARCHAR50  PatientLoginID PK
        VARCHAR50  DoctorLoginID  PK
        VARCHAR50  TimeSlot时间段
    }

    %% 关系
    Patient ||--o{ Appointment : "makes"
    Doctor  ||--o{ Appointment : "accepts"
    Admin   ||--o{ Doctor      : "manages"
    Admin   ||--o{ Patient     : "manages"


```