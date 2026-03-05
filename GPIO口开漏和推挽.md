## 什么是Mos管
- MOS 管 = 金属 - 氧化物 - 半导体场效应管，是电子里最常用、最重要的开关 / 放大元件，相当于电控开关。
<img width="158" height="156" alt="image" src="https://github.com/user-attachments/assets/ab763047-5927-406f-bc62-09dfabebffa9" />

## 推挽输出
我们直接上图看原理<br>
<img width="782" height="724" alt="image" src="https://github.com/user-attachments/assets/faba329b-76e8-4c95-a003-914dd3b24dbc" />
当Q1触发，Q2没有触发时，输出端口连通VCC输出高电平<br>
当Q1没有触发，Q2触发时输出端连通GND输出端口输出低电平

## 开漏输出
- 简单来说开漏输出就是单片机不能自己拉高电平，只能通过外部电路拉高电平
<img width="650" height="748" alt="image" src="https://github.com/user-attachments/assets/b98bea06-7f56-4ba2-932f-5ca3dc558b6a" /><br>
原理看图可知当mos管连通时单片机对外输出低电平，单片机只能将电平拉低，当mos管不连通时单片机处于高阻态单片机通过外部上拉电阻输出高电平
- 开漏输出还解决了个问题，相较于推挽输出，单片机只能输出5V高电平对于一些对电平要求为5V一下的芯片来说与之连通容易造成芯片的烧坏，而开漏输出则可已通过接入对应电压的电阻从而实现输出对应的电压

## 什么是高阻态？
- 高阻态 = 既不输出高电平，也不输出低电平，相当于 “断开、悬空”。


