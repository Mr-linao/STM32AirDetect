## **安装EMQX：**

### 通过docker安装[参考链接](https://docs.emqx.com/zh/emqx/latest/deploy/install-docker-ce.html)

```
docker pull emqx/emqx:5.8.5
```

```
docker run -d --name emqx -p 1883:1883 -p 8083:8083 -p 8084:8084 -p 8883:8883 -p 18083:18083 emqx/emqx:5.8.5
```

### 通过APT源安装[参考链接](https://docs.emqx.com/zh/emqx/latest/deploy/install-ubuntu-ce.html)

通过以下命令配置 EMQX Apt源：

```
curl -s https://assets.emqx.com/scripts/install-emqx-deb.sh | sudo bash
```

运行以下命令安装 

```
EMQX：sudo apt-get install emqx
```

运行以下命令启动 EMQX：

```
sudo systemctl start emqx
```

#### 运行Dashboard管理控制台

打开浏览器输入 `http://localhost:18083` （localhost为你的设备IP）即可打开Dashboard管理控制台，默认账号：`admin`密码：`public`。首次使用需要修改密码。
在EMQX安装完成后记得放行相关端口，否则Dashboard管理控制台无法访问。
