# 自定义的镜像基于ubuntu:14.04
FROM ubuntu:14.04

# 设置环境变量
ENV DIRPATH /root

# 复制编译后的文件到制定目录，/root/hello
COPY hello $DIRPATH/hello

# 端口
EXPOSE 5000

# 制定工作目录
WORKDIR $DIRPATH

# 运行程序
CMD ./hello
