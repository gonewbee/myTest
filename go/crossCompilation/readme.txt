# go1.5 交叉编译参考：http://dave.cheney.net/2015/03/03/cross-compilation-just-got-a-whole-lot-better-in-go-1-5

# 编译windows amd64位系统
env GOOS=windows GOARCH=amd64 go build -v hello.go

# build for linux/arm
env GOOS=linux GOARCH=arm GOARM=7 go build hello.go
