## 在windows上编译linux应用
参考[How to cross compile from Windows to Linux?](http://stackoverflow.com/questions/20829155/how-to-cross-compile-from-windows-to-linux)
```sh
set GOARCH=amd64
set GOOS=linux
go build .
```
