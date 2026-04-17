import itchat
from itchat.content import TEXT

itchat.auto_login(hotReload=False)
@itchat.msg_register(TEXT)
def simple_reply(msg):
    print(msg)
    return f"收到消息:{msg.Text}\n"
print("脚本已启动，扫码登录后即可自动回复～（按Ctrl+C退出）")
itchat.run()