## **智能室内气体检测系统**

### **项目简介**

本系统基于STM32F103C8T6微控制器核心，实时监测室内环境参数（温湿度/空气质量），具备本地显示、智能调控与云端交互三大核心功能。通过MQTT协议上传至HomeAssistant实现数据可视化和硬件控制，可接入主流智能家居生态，例如与小爱同学联动控制灯光以及排气系统开关。

![](https://github.com/Mr-linao/STM32AirDetect/blob/master/Doc/img/Air%20Detector001.png)

### 主要功能

📊 实时监测：温湿度、空气质量（ppm值）

🖥️ 本地显示：SSD1306 OLED屏幕实时展示数据

🌀 自动控制：异常时启动排气扇和窗户

☁️ 云端交互：通过HomeAssistant远程查看数据、控制设备

🔔 报警逻辑：温度≥35°C / 湿度≥80% / 污染≥150ppm时触发

### **硬件组成**

| 模块      | 型号            | 功能说明     |
|:-------:|:-------------:|:--------:|
| 主控芯片    | STM32F103C8T6 | 系统核心控制   |
| 温湿度传感器  | DHT11         | 采集环境温湿度  |
| 空气质量传感器 | MQ135         | 检测空气质量   |
| 电机驱动模块  | MX1508        | 驱动电机     |
| 舵机      | SQ90          | 控制窗户开合角度 |
| 显示屏     | SSD1306 OLED  | 本地数据显示   |
| WiFi模块  | ESP8266-01S   | 无线网络连接   |

### **云端部署**

#### 系统环境

1. 云服务器（腾讯云 Ubuntu 20.04 LTS）
2. Docker（Docker version 27.4.1）
3. HomeAssistant + EMQX（MQTT服务器）

[安装Docker](./Doc/01安装docker.md)

[EMQX安装](./Doc/02安装EMQX.md)

[HomeAssistant安装](./Doc/03安装HomeAssistant.md)（为了方便阅读下文简称HA）

[米家与HA集成](./Doc/05米家与HA集成.md)

在EMQX接入完毕HA后需要添加实体实现数据上传以及硬件开关控制功能。这里需要编辑**HA**的**configuration.yaml**文件，在终端执行命令` vi home/configuration.yaml`（以你文件实际目录为准）按下键盘`"i"`进入编辑模式，在左下角状态栏中会出现` –INSERT- `的字样，在空白行添加以下内容：

```
mqtt:
  switch:
  - name: "FAN Switch"      #设备名称
    unique_id: FAN          #设备ID
    command_topic: "STM32IOT/FAN"       # HA向设备发送控制命令的MQTT主题
    state_topic: "STM32IOT/FAN_State"   #设备向HA上报状态的MQTT主题
    payload_on: '{"FAN_Switch":1}'		#HA向设备下发“开”状态
    payload_off: '{"FAN_Switch":0}'		#HA向设备下发“关”状态
    state_on: "1"   #设备上报“开”状态
    state_off: "0"  #设备上报“关”状态
    
  light:
    - name: "LED Switch"
      state_topic: "STM32IOT/LED_State"
      command_topic: "STM32IOT/LED"
      payload_on: '{"LED_Switch":1}'
      payload_off: '{"LED_Switch":0}'
      unique_id: "LED"
      optimistic: false
    
  sensor:
    - name: "Temperature"
      state_topic: "STM32IOT/Temp"
      suggested_display_precision: 1
      unit_of_measurement: "°C"
      value_template: "{{ value.split(':')[1] }}"
      
      
    - name: "Humidity"
      state_topic: "STM32IOT/Humi"
      unit_of_measurement: "%"
      value_template: "{{ value.split(':')[1] }}"
     
    - name: "空气质量传感器"
      state_topic: "STM32IOT/MQ135"
      suggested_display_precision: 1
      unit_of_measurement: "ppm"
      value_template: "{{ value.split(':')[1].split(' ')[0] }}"
```



添加完成后按下ESC键回到一般模式，左下角状态栏中–INSERT- 的字样消失，输入:wq回车保存。

<img src="G:\C-Projects\STM32Project\智能室内气体检测系统\Doc\img\HA_Config001.png" style="zoom:50%;" />

Tips：如果你不知道这个文件在哪里 可以使用` find / -name "configuration.yaml" `命令全盘查找，但这会非常慢。

### **硬件端配置**

#### 硬件连接

| 模块             | 引脚   |
|:--------------:|:----:|
| DHT11          | PA0  |
| MQ135          | PA1  |
| ESP8266/RX     | PA2  |
| ESP8266/TX     | PA3  |
| ESP8266/RST    | PA4  |
| LED            | PA5  |
| MX1508电机驱动/IN1 | PA6  |
| MX1508电机驱动/IN2 | PA7  |
| SQ90           | PA8  |
| Ch340串口模块/RX   | PA9  |
| Ch340串口模块/RX   | PA10 |
| OLED/SCL       | PB8  |
| OLED/SDA       | PB9  |

#### 代码：

相关代码参考目录树

```
.
├── DebugConfig
├── Doc
├── Library
├── Listings
├── Objects
├── Start
├── Hardware
│   ├── 硬件驱动文件
├── System
│   ├── 底层核心驱动文件
├── User
│   ├── 主函数
├── Project.uvguix.Admin
├── Project.uvguix.Oreo
├── Project.uvoptx
├── Project.uvprojx
├── README.md
└── keilkill.bat

```



#### 代码修改：

由于现阶段技术能力尚浅，物联网平台官方文档涉及到的内容较为复杂，学习门槛较高，物联网方面的代码使用的是张继瑞大佬的OneNET源码。相关文件有很详细的注释，这里仅需要修改一下部分文件即可。

**`esp8266.c`**

```
#define ESP8266\_WIFI\_INFO        "AT+CWJAP=\"Leon\",\"12345678\"\r\n"    //修改为自己的WiFi信息，Leon为WiFi名称，12345678为密码，注意使用2.4GHz。

#define ESP8266\_ONENET\_INFO        "AT+CIPSTART=\"TCP\",\"xx.xx.xx.xx\",1883\r\n"    //修改自己的服务器IP，端口如果没有特别设置则为1883
```

**`Onenet.c`**

```
#define PROID        "xxxxx"    //产品ID，如果使用EMQX，产品ID就是用户名

#define AUTH\_INFO    "xxxxx"    //产品鉴权信息，如果使用EMQX，产品鉴权信息就是密码

#define DEVID        "xxxxx"    //设备ID，如果使用EMQX，设备ID就是客户端ID
```

