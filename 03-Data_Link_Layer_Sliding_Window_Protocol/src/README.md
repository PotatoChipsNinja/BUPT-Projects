### 使用方法
1. 将 `(gobackn)datalink.c`（回退N步协议）或 `(selective)datalink.c`（选择重传协议）重命名为 `datalink.c`
2. 覆盖程序库中的 `datalink.c`（原始内容为为停-等协议）
3. Linux 下执行 `make`，或在 Visual Studio 下进行编译
4. 在两个 shell 中分别执行 `./datalink A` 和 `./datalink B`，协议即可开始工作，其他运行参数请见手册