# 测试用例

* 循环控制流
* if-else
* 函数嵌套
* 多次报错

## 测试错误恢复

`./test/witherr.woc` 其中构造了四个错误。

局限：无法恢复括号不配对的情况。

```
$ ./woc ./tests/witherr.woc -a
8: syntax error, unexpected RTN, expecting SEM
14: syntax error, unexpected ELS
19: syntax error, unexpected ASN, expecting RPAREN
26: syntax error, unexpected RBRACE, expecting SEM
Unable to generate AST for file ./tests/witherr.woc
```
