# cpp-qt-medical-system

北京理工大学 23 级计科大三小学期计算机专业基础实习大作业

## 目录

1. [项目概述](#1-项目概述)
2. [团队分工](#2-团队分工)
3. [模块划分与任务分配](#3-模块划分与任务分配)
4. [架构设计与接口约定](#4-架构设计与接口约定)
5. [初步时间计划](#5-初步时间计划)
6. [开发规范](#6-开发规范)
7. [可供参考的资料](#7-可供参考的资料)

---

## 1. 项目概述

### 1.1 项目目标

9 月 2 日中午之前，参照[项目说明书](./docs/00[新]项目说明书-智慧医疗管理系统V2.pdf)完成 [1.3](#13-最终需要产出的成果) 中的所有内容，并在有余力的情况下完成我们新增的需求以获取加分。

### 1.2 技术栈

- **开发环境:** Ubuntu 18.04
- **框架:** Qt 5.12.8
- **编译器:** GCC 7.5.0
- **编程语言:** C++
- **数据库:** SQLite

### 1.3 最终需要产出的成果

- **可执行程序:** 打包好的应用程序，用于路演展示用。
- **项目源码:** 所有源代码和资源。
- **项目文档:** 校发的 10 个左右的项目文档资料。

---

## 2. 团队分工

### 2.1 角色分配

| 姓名 | 角色 | 主要职责 |
| :--- | :--- | :--- |
| **左逸龙** | 文档 / TL | 组织协调、进度跟踪、文档整合、辅助开发 |
| **刘兆钰** | 文档 / 测试 / SMCL | 文档攥写、功能测试、UI 测试 |
| **周鑫** | **后端 & 架构主力** | 数据库设计、后端业务逻辑、核心API接口实现、管理员后端(在时间允许的情况下) |
| **胡艺镭** | **前端 & UI 主力** + PRL | 患者端/医生端/管理员端所有UI设计与实现、用户交互逻辑、CR (Code Review) |
| **黄奕晨** | **功能模块 & 算法主力** | 医患聊天、病历模板、数据分析与可视化等独立功能模块开发 |

> 需要申明的是，分工并不是死板的，我们更多的是线下开发，因此在开发过程当中可以灵活调整，也可以互相帮助。

### 2.2 沟通机制

- **线下讨论:** 在教室坐在一起，可以随时交流。
- **微信聊天:** 其他时候使用微信聊天。
- **腾讯会议:** 根据实际情况不定时召开会议 (可能没有)。

---

## 3. 模块划分与任务分配

### 3.1 总体模块视图

```mermaid
graph TD
    %% --- Actors ---
    subgraph "Actors"
        direction LR
        Doctor(fa:fa-user-doctor Doctor)
        Patient(fa:fa-user Patient)
        Admin(fa:fa-user-shield Admin)
    end

    %% --- App Architecture ---
    subgraph "Application Architecture"
        direction TB

        %% 1. UI（客户端）
        subgraph "1.UI (Client)"
            direction LR
            LoginUI["登录/注册"]
            PatientUI["患者端"]
            DoctorUI["医生端"]
            AdminUI["后台端"]
        end

        %% 2. 业务层（客户端）
        subgraph "2.Biz (Client)"
            direction TB
            Controller{"UiController（核心控制）"}
            subgraph "功能模块 (Features)"
                direction TB
                ChatModule["医患聊天"]
                AnalysisModule["数据分析"]
                TemplateModule["病历模板"]
            end
        end

        %% 3. 后端（仅数据访问）
        subgraph "3.Backend (Server)"
            direction TB
            ServerAPI{"Server API / DataProxy"}
            subgraph "数据访问 (Data Access)"
                direction TB
                DBModule["Database (SQLite)"]
            end
        end

        %% 4. 数据模型
        subgraph "4.Tables"
            direction LR
            UserInfo["用户信息"]
            MedicalRecords["病历信息"]
            Appointments["预约信息"]
        end
    end

    %% --- Connections ---
    Doctor & Patient & Admin --> LoginUI
    LoginUI --> Controller
    PatientUI --> Controller
    DoctorUI --> Controller
    AdminUI --> Controller

    Controller --> ChatModule
    Controller --> AnalysisModule
    Controller --> TemplateModule

    Controller -->|HTTPS| ServerAPI
    ServerAPI <--> DBModule

    DBModule -- Manages --> UserInfo
    DBModule -- Manages --> MedicalRecords
    DBModule -- Manages --> Appointments

    %% --- Styling（紧凑+深灰字+稍大字号）---
    style Doctor fill:#d4edda,stroke:#155724
    style Patient fill:#d1ecf1,stroke:#0c5460
    style Admin fill:#f8d7da,stroke:#721c24

    classDef actor color:#333,font-size:14px,stroke-width:1.2px
    classDef ui fill:#cce5ff,stroke:#004085,color:#333,font-size:14px,stroke-width:1.2px
    classDef logic fill:#fff3cd,stroke:#856404,color:#333,font-size:14px,stroke-width:1.2px
    classDef feature fill:#e2e3e5,stroke:#383d41,color:#333,font-size:14px,stroke-width:1.2px
    classDef data fill:#d4edda,stroke:#155724,color:#333,font-size:14px,stroke-width:1.2px

    class Doctor,Patient,Admin actor
    class LoginUI,PatientUI,DoctorUI,AdminUI ui
    class Controller logic
    class ChatModule,AnalysisModule,TemplateModule feature
    class ServerAPI,DBModule,UserInfo,MedicalRecords,Appointments data

    linkStyle default stroke-width:1.1px

```

### 3.2 详细任务分配表

> 参考 [12-04需求理解一览表.xls](./docs/12-04需求理解一览表.xls) 的需求理解来完成，但是不要参考它的分工。详细分工如下：

#### 👨‍💻 **周鑫 (后端 & 架构主力)**

| 职责分类 | 详细任务 | 相关需求 ID |
| :--- | :--- | :--- |
| **数据库** | 1. **设计与实现:** 根据讨论情况，设计并创建所有数据表。 <br> 2. **数据接口:** 编写`DatabaseManager`类，提供所有原子化的数据库CRUD操作。 | 17, 18, 19, 20, 21, 22, 23 |
| **核心架构** | 1. **控制器设计:** 定义`UiController.h`的全部接口，作为系统唯一逻辑入口。 <br> 2. **后端服务实现:** 将所有后端逻辑封装在`UiController`的`.cpp`文件中。 | 27 |
| **用户与账户** | 1. **注册后端:** 校验邮箱唯一性，密码加密存储，创建新用户。 <br> 2. **登录后端:** 验证用户凭据，返回登录状态和用户信息。 <br> 3. **密码策略:** 实现密码复杂度校验逻辑。 <br> 4. **找回密码后端:** 处理邮箱/验证码验证逻辑。 | 1 (后端), 2 (后端), 29, 30 (后端) |
| **业务逻辑** | 1. **个人信息后端:** 提供接口以更新和获取用户（医/患）的详细信息。 <br> 2. **挂号预约后端:** 处理挂号请求，校验医生排班，原子化更新预约数。 <br> 3. **病历/医嘱后端:** 提供接口以创建、查询和更新病历、医嘱、处方。 <br> 4. **预约提醒后端:** 提供接口查询用户24小时内的预约。 | 3 (后端), 4 (后端), 5 (后端), 6, 7, 8, 9, 14, 15, 16 (后端), 32, 33 (后端), 34 |
| **管理员模块** | 1. **后台接口:** 实现对所有用户的增、删、改、查管理接口。 | 45 (后端) |

#### 🎨 **胡艺镭 (前端 & UI 主力)**

| 职责分类 | 详细任务 | 相关需求 ID |
| :--- | :--- | :--- |
| **UI设计与实现** | 1. **界面开发:** 负责**所有**窗口和自定义控件的UI设计与实现(.ui文件+代码)。包括登录/注册、医/患/管主界面、信息编辑、挂号、病历、考勤等所有可见界面。 | 贯穿所有UI相关需求 |
| **前端交互** | 1. **信号与槽:** 将所有UI控件的信号（如按钮点击）连接到`UiController`的槽函数。 <br> 2. **数据绑定:** 调用`UiController`的接口获取数据，并将其展示在UI界面上。 <br> 3. **状态处理:** 根据`UiController`返回的结果（成功/失败/具体错误），在UI上给用户合适的反馈（弹窗、提示信息等）。 | 贯穿所有UI相关需求 |
| **患者端流程** | 1. **核心流程:** 实现从注册、登录到查看医生、挂号、查看病历、医嘱、处方的完整用户流程的UI。 <br> 2. **预约提醒:** 在主界面加载时调用后端接口，并用一个优雅的、非侵入式的方式（如通知栏）展示提醒信息。 | 1, 2, 3, 4, 5, 6, 7, 8, 9, 14, 15, 16 |
| **医生端流程** | 1. **核心流程:** 实现医生登录、编辑信息、查看预约患者、管理病历、考勤打卡的完整流程的UI。 | 30, 31, 32, 33, 34, 37, 38, 39, 40, 41 |
| **管理员流程** | 1. **管理界面:** 实现用户列表的展示、搜索，以及添加/重置/禁用用户的操作界面。 | 45 |
| **CR(Code Review)** | 1. **代码审查:** 对其他两位同学的 PR 进行审查，确保代码质量。 | 贯穿所有代码 |

#### 🧩 **黄奕晨 (功能模块 & 算法主力)**

| 职责分类 | 详细任务 | 相关需求 ID |
| :--- | :--- | :--- |
| **医患沟通** | 1. **聊天逻辑:** 实现`ChatMessage`数据结构，以及通过`UiController`发送和接收消息的逻辑。 <br> 2. **聊天UI集成:** 与周鑫协作，将聊天逻辑嵌入到聊天窗口UI中，实现消息的实时显示。 <br> 3. **语音识别:** 实现调用系统API或一个简单的模拟功能，将语音输入转换为文字并填入输入框。 | 10, 43, 44 |
| **数据分析** | 1. **图表生成:** 使用Qt Charts或其他方式，获取`UiController`提供的数据，并动态生成基础数据分析图表（如折线图）。 <br> 2. **远端数据采集:** **模拟**一个智能心电仪API调用，获取预设的JSON或XML数据，并进行解析和分析。 | 35, 36 |
| **效率功能** | 1. **药品搜索:** 实现一个本地药品搜索功能，根据用户输入在预设的药品列表中进行筛选并展示结果。 <br> 2. **病历模板:** 实现调用`UiController`接口获取病历模板，并将其内容填充到当前病历编辑界面的逻辑。 | 12, 42 |
| **支付模块** | 1. **线上支付:** **模拟**微信支付流程。点击支付后，显示一个二维码图片，并提供一个“我已支付”按钮来模拟回调，以继续流程。 | 13 |
| **其他服务** | 1. **邮箱验证:** 实现邮件发送功能以验证邮箱或找回密码。 <br> 2. **网关与状态服务:** 提供网关与状态监控服务。 <br> 3. **缓存机制:** (可选，若时间允许) 为高频读取的数据（如科室列表）实现简单缓存。 | 24, 25, 26 |

---

### 无法实现的需求

以下是一些经过分析后发现不切实际或无法完成的需求，进行了一定的修改。

| 需求 ID | 需求名称 | 无法完成/不合理的原因 | 方案 |
| :--- | :--- | :--- | :--- |
| **28** | **分布式部署云服务器** | 这属于运维(DevOps)范畴，需要购买云服务器、配置环境、学习部署流程，远超本次开发实习的范围和时间限制。 | **目标:** 交付一个可运行的程序。<br>**方案:** 提供一个打包好的本地桌面应用程序（Windows的`.exe`或Linux的可执行文件）及安装说明。 |
| **13** | **线上支付** | 对接真实的微信支付API需要企业资质认证、复杂的SDK集成和后端回调处理，无法在短期内完成。 | **目标:** 体验支付流程。<br>**方案:** **模拟支付**。显示二维码图片并提供一个按钮来手动确认支付成功。 |
| **36** | **远距数据采集分析** | 我们没有真实的智能心电仪硬件或其配套的API服务。 | **目标:** 展示处理外部API数据的能力。<br>**方案:** **模拟API**。创建一个函数或读取本地文件，使其返回一段预设好的、模仿心电仪数据的JSON字符串，然后对此数据进行解析和展示。 |

---

## 4. 架构设计与接口约定

### 4.1 数据库设计 (ER图)

详情请参考[另外一个文档](./ER.md)

### 4.2 核心架构与数据模型

- 我们将采用一个**单例**的 `UiController` 类作为前后端交互的唯一入口，以实现逻辑解耦。
- 前后端之间的数据传递将使用通用的 `QVariantMap`, `QVariantList` 等完成。

### 4.3 前后端交互 (`UiController`)

- 我们使用 `UiController` 来完成前后端之间的交互。
- 关于 `UiController` 当中函数功能的实现，请与 **周鑫** 商量讨论后，由 **周鑫** 负责具体实现，不用自己上手来完成。
- 需要用到的 Qt 信号同理，由 **周鑫** 来具体实现，其他人只需要提需求即可。

---

## 5. 初步时间计划

```mermaid
gantt
    title 计算机专业基础实习项目
    dateFormat YYYY-MM-DD HH:mm

    section 架构设计
    设计讨论 :crit, task_design, 2025-08-29 00:00, 12h
    
    section 核心开发
    后端API开发 (周) :crit, task_backend, after task_design, 60h
    前端UI开发 (胡) :task_frontend, after task_design, 60h
    功能模块开发 (黄) :task_features, after task_design, 60h

    section 集成与测试
    前后端功能联调 :crit, task_integ, 2025-08-31 12:00, 24h
    功能测试 :task_test, 2025-09-01 00:00, 36h
    Bug修复 & 体验优化 :crit, task_bugfix, 2025-09-01 12:00, 24h

    section 其他
    文档完善 & PPT 制作 :task_deliver, 2025-08-29 00:00, 2025-09-02 12:00
```

> 实际情况可能会有所变化，但大致上会遵循这个时间计划。

---

## 6. 开发规范

### 6.1 Git 工作流

- **分支命名:**
  - 功能开发: `feature/user-login` 或 `feat/chat-module`
  - Bug修复: `fix/login-crash`
  - ......
- **提交信息:**
  - 尽量遵循 [Conventional Commits](https://www.conventionalcommits.org/) 规范。格式: `<type>: <subject>`
- **合并流程:**
  - 禁止直接 `push` 到 `main` 分支，容易导致代码冲突。
  - 所有代码通过 **Pull Request (PR)** 合并，需要至少一人进行 **Code Review**。

### 6.2 代码风格

- **命名规范:**
  - 类名: `UpperCamelCase` (e.g., `PatientModel`)
  - 函数/变量名: `lowerCamelCase` (e.g., `getUserName`)
  - 私有成员变量: 加 `m_` 前缀 (e.g., `m_userName`)
- **缩进规范**
  - 统一采用 `Allman Style`，主要是左大括号需要下放到下一行
  - 统一使用四个空格的缩进长度，如果 `Tab` 缩进长度不同的可能需要调整一下
- **其他的规范可以后续继续补充**

---

## 7. 可供参考的资料

> 有的代码可以直接照抄过来，尤其是聊天相关的代码，前人有较多具体实现。

- [一个同样也是 Qt + Cpp 开发的医院管理系统，推荐参考](https://github.com/OmerJauhar/Hospital-Management-System-HMS-QT-Software-OOP-C-)
- [使用 Qt + Cpp 开发的实时聊天系统，推荐参考](https://github.com/vRFEducation/qtchatapplication)
- 23级其他组正在进行当中的项目：
  - [同样也是医疗系统，流程图画得比较清晰，可以参考](https://github.com/xialinguo/hospital)
  - [目前为空，等待后续动作](https://github.com/Dingnuooo/bit2025xxq)
- 往年学长学姐们的项目 (之前的项目有所区别，不过也可以参考)：
  - [22级车载系统，含有聊天模块，可以参考其他文档攥写](https://github.com/YYT-0901/CS-BIT-INFORMATION/tree/master/5semester%20%E5%A4%A7%E4%B8%89%E4%B8%8A/%E8%AE%A1%E7%AE%97%E6%9C%BA%E4%B8%93%E4%B8%9A%E5%9F%BA%E7%A1%80%E5%AE%9E%E4%B9%A0)
  - [同样也是车载系统](https://github.com/xChang1021x/BIT/tree/main/Y3S1/%E8%AE%A1%E7%AE%97%E6%9C%BA%E4%B8%93%E4%B8%9A%E5%9F%BA%E7%A1%80%E5%AE%9E%E4%B9%A0)
  - 更早一些的时候，单纯只用做一个聊天软件，基本都采用 [Client](https://github.com/muxinyu1/q-chater-client) + [Server](https://github.com/muxinyu1/q-chater-server) 的架构
  - 同样只做聊天软件：[Client](https://github.com/loujuch/client) + [Server](https://github.com/loujuch/server)
  - 同样只做聊天软件：[Client+Server](https://github.com/zx1316/linpop-lan-im)