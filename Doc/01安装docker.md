## **以Ubuntu为例安装Docker：**

如果不确定有没有安装过的话可以使用以下命令查看版本号。

```
docker --version
```

#### **1. 更新系统包索引**

首先，确保系统包索引是最新的。打开终端并运行以下命令：

```
sudo apt update
```

#### **2. 安装必要的依赖包**

 确保系统中安装了 `apt-transport-https`、`ca-certificates` 和 `curl`等必要的依赖包：

```
sudo apt install apt-transport-https ca-certificates curl software-properties-common
```

#### **3. 添加 Docker 官方 GPG 密钥**

   添加 Docker 官方的 GPG 公钥，用于验证软件包的来源：

```
curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo gpg --dearmor -o /usr/share/keyrings/docker-archive-keyring.gpg
```

#### **4. 添加 Docker 官方 APT 仓库**

   将 Docker 的 APT 仓库添加到系统中：

```
echo "deb [arch=amd64 signed-by=/usr/share/keyrings/docker-archive-keyring.gpg] https://download.docker.com/linux/ubuntu $(lsb_release -cs) stable" | sudo tee /etc/apt/sources.list.d/docker.list > /dev/null
```

#### **5. 更新包索引并安装 Docker 引擎**

更新包索引以包含新添加的 Docker 仓库：

```
sudo apt update
```

安装 Docker 引擎、容器工具和 docker-compose 插件：

```
sudo apt install docker-ce docker-ce-cli containerd.io docker-buildx-plugin docker-compose-plugin
```

#### **6. 启动并验证 Docker 服务**

启动 Docker 服务并使其开机自启：

```
sudo systemctl start docker
sudo systemctl enable docker 
```

#### 查看 Docker 版本信息：

```
docker --version
```

#### 运行一个测试容器来验证 Docker 是否工作正常：

```
sudo docker run hello-world
```

如果 Docker 正常工作，输出将显示从 `hello-world` 镜像生成的消息，表示 Docker 已经成功安装并运行。