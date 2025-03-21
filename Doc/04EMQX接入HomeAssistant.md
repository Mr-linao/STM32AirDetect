## **EMQX接入HomeAssistant：**

在 Home Assistant 中，需要启用高级模型才能使用 MQTT 集成中的高级功能，主要是为了更换 MQTT 协议为版本 5，默认是 3.1.1 接入。在个人资料中，点击 高级模式 开关，然后重启 Home Assistant 即可启用高级模式。

<img title="" src="file:///G:/C-Projects/STM32Project/智能室内气体检测系统/Doc/img/EMQX_HA001.png" alt="" width="653" data-align="center"> 

### **安装MQTT集成：**

在 Home Assistant 中，导航到 设置 -> 设备与服务，然后点击右下角的 + 添加集成 按钮，搜索并选择 MQTT**。**

<img src="file:///G:/C-Projects/STM32Project/智能室内气体检测系统/Doc/img/EMQX_HA002.png" title="" alt="" data-align="center">

在添加 MQTT 集成时，需要输入 EMQX 服务器的信息，然后打开高级选项，点击下一步。

<img title="" src="file:///G:/C-Projects/STM32Project/智能室内气体检测系统/Doc/img/EMQX_HA003.png" alt="" width="250" data-align="center">

此时，Home Assistant 将会连接到EMQX服务器，你可以在EMQX管理后台的监控中看到 MQTT 设备的状态，验证是否成功连接。

![](G:\C-Projects\STM32Project\智能室内气体检测系统\Doc\img\EMQX_HA004.png)

至此已经成功实现EMQX与Home Assistant的对接。
