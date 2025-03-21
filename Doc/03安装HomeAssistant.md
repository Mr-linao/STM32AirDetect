## **HomeAssistant安装：**

通过docker安装（非唯一方式，这里仅作举例）：

```
docker pull homeassistant/home-assistant:latest
```

```
docker run -d \
  --name homeassistant \
  --restart=unless-stopped \
  -v /path/to/your/config:/config \
  -e "TZ=Asia/Shanghai" \
  --net=host \
  homeassistant/home-assistant:latest
```

其中，`/path/to/your/config`是您希望存储 HomeAssistant 配置文件的路径。同样需要放行相关端口。打开浏览器，访问 `http://localhost:8123`（localhost为你的IP），你将看到 HomeAssistant 的初始化界面。完成HA要求一些账号密码配置即可。
