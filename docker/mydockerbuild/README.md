### 生成镜像
+ 编译应用 go build hello.go
+ 生成镜像 docker build -t docker-hello .
+ 运行测试 docker run -d -P docker-hello

### 修改镜像
+ 修改hello.go后，将hello复制到container中
    
    docker cp hello b016e223fc44842ddc1f38a4c5fbc07d459656988554b15bff6531f4270cf70b:/root/hello

+ 提交修改

    docker commit b016e223fc44842ddc1f38a4c5fbc07d459656988554b15bff6531f4270cf70b docker-hello
