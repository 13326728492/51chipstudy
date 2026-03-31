# 8051内存架构说明
<br>  
实现这个小内核之前我觉得有必要先说明一下该单片机的内存架构，这里我主要介绍两部分：<br>

- Flash 用于放代码
- 内部RAM（重点）
由于平时写代码主要操作的是内部的RAM

## Flash
flash是单片机内存放执行代码的地方，也包括任务执行的代码
CPU从这里通过PC（程序计数器）在flash里取指令然后执行
和下面RAM不一样的是这玩意是==16为寻址==

## 内部RAM
内部的ram就是我们代码主要操作的地方（主要是想说一下这里的使用到寄存器的分布），而且与上面不一样的是，这支持==8位的寻址==，这意味这这个内部的RAM打死最大也不会超过256Byte，这个RAM是定死256字节，接下来我们看看这256BYte色架构图：

![4d4e352d3ee57b84ea5375d33db92174~tplv-be4g95zd3a-image](https://github.com/user-attachments/assets/6f2b6df5-376b-409f-a788-69205f9dce34)

  
-这里256字节主要分为两部分：高128Byte和低128Byte
### 高128字节 80H~FFH
-这里有一个非常需要注意的点是这高128字节，是两块同为128Byte的物理内存的重叠的地方（一块是SFR，一块是高128Byte内部RAM），也就是说他们的地址是一样的，**那问题来了，我们有应该怎么去访问这两个不同的内存呢。不墨迹直接说，其实就是使用不同的寻址方式就能访问到对应的内存下面我给一点代码示例：**

- 直接寻址访问SFR
```nasm
MOV 90H,#55H   ; 写P1口（90H是P1的SFR地址）
MOV A,0E0H     ; 读ACC（0E0H是ACC的SFR地址）
```
- 简介寻址访问高128Byte internal RAM
```nasm
MOV R0,#80H    ; 指针指向80H
MOV @R0,#33H   ; 写8052的80H额外RAM
MOV 20H,@R0    ; 读8052的80H额外RAM
```
很明显前者是直接对这个值进行操作，而后者则是把值放入通用寄存器之后再通过这个通用寄存器来进行操作，这个例子使用到的通用寄存器是R0
- 这里我简单罗列一下这里SFR里尝试用的端口：
 ***80H***  &nbsp;&nbsp; P0  
 ***81H***  &nbsp;&nbsp; SP  
 ***82H***  &nbsp;&nbsp; DPL  
 ***83H***  &nbsp;&nbsp; DPH  
 ***87H***  &nbsp;&nbsp; PCON
 ***88H***  &nbsp;&nbsp; TCON  
***89H***  &nbsp;&nbsp; TMOD 
***8aH***  &nbsp;&nbsp; TL0 
***8bH***  &nbsp;&nbsp; TL1 
***8cH***  &nbsp;&nbsp; TH0 
***8dH***  &nbsp;&nbsp; TH1  
***90H***  &nbsp;&nbsp; P1  
***98H***  &nbsp;&nbsp; SCON
***99H***  &nbsp;&nbsp; SBUF  
***A0H***  &nbsp;&nbsp; P2  
***B0H***  &nbsp;&nbsp; B3 
***D0H***  &nbsp;&nbsp; PSW 
***E0H***  &nbsp;&nbsp; ACC  
***F0H***  &nbsp;&nbsp; B  
  
其中SP是栈指针寄存器，而DPL DPH是索引外设内存的寄存器这里分别是第八位和高八位,PSW是程序状态寄存器这是x86
内的EFLAG的迷你版，这里顺带提两嘴，其他的可以查阅参考手册
### 低128Byte的地址
- 首先我们先看00H~20H：
  这里分别是八个通用寄存器R0 R1 R2 R3 R4 R5 R6 R7 对应的RAM地址
- 接下来是20H~2FH的可位寻址区，什么说是可位寻址区？
  这玩意有点类似位图，什么是位图呢，这可以deepseek一下，这里是一个硬件级的位图，这里可以支持一位一位的寻址：
  take an example 20H.0就是指的是第二十字节的第零位
- 30H~7FH
  这一段内存正常情况下用作栈但CPU的认知里是不知道什么是栈底的这取决你怎么对SP寄存器进行操作但是没有特殊情况下，一般都将此段内存作为栈

- 号内存就介绍到这里
