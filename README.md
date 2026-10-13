# ESP32-S3 Voice Light Control

这是一个用于课程设计复现的 ESP-IDF 工程：ESP32-S3 接收语音识别模块给出的命令 ID，控制灯带开关、亮度，并可将状态发布到 MQTT 云端。

## 已实现的控制逻辑

| 命令 ID | 语音命令 | 固件动作 |
| --- | --- | --- |
| 1 | 打开灯光 | 开启 PWM 灯带输出 |
| 2 | 关闭灯光 | 关闭 PWM 灯带输出 |
| 3 | 增大亮度 | 亮度加 15%，最大 100% |
| 4 | 减小亮度 | 亮度减 15%，最小 1% |
| 5 / 6 / 7 | 一 / 两 / 三小时后自动关灯 | 以最新任务覆盖旧定时任务 |

`main/light_controller.c` 是硬件执行逻辑，`main/command_router.c` 负责命令路由，`main/mqtt_bridge.c` 负责可选 MQTT 状态上报，`main/voice_commands.c` 是 ESP-SR 识别回调的对接点。

## 配置与烧录

1. 安装 ESP-IDF 5.5 或兼容版本。
2. 在工程目录执行：

   ```bash
   idf.py set-target esp32s3
   idf.py menuconfig
   idf.py build flash monitor
   ```

3. 在 `menuconfig -> Voice light control` 设置灯带 PWM GPIO 和亮度默认值。
4. 如需云端上报，启用 `Publish light state to MQTT`，再填入个人服务器 MQTT URI 和主题。不要提交服务器账号、密码或密钥。
5. 根据实际 ESP32-S3 开发板配置 PDM/I2S 麦克风引脚，并在 ESP-SR 的 MultiNet 回调中调用：

   ```c
   voice_commands_handle_recognition(command_id, recognized_text);
   ```

## ESP-SR 对接说明

本仓库保留了灯光控制、亮度限幅、定时覆盖和 MQTT 状态发布的项目代码。语音前端需要按开发板型号配置，因原始硬件引脚和麦克风型号未公开，仓库不猜测这些参数。

在 ESP-SR MultiNet 初始化阶段，`voice_commands_register` 已注册命令词：`打开灯光`、`关闭灯光`、`增大亮度`、`减小亮度`、`一小时后自动关灯`、`两小时后自动关灯`、`三小时后自动关灯`。识别成功后，将命令 ID 和文本传入 `voice_commands_handle_recognition`。如需更多时长，可按相同方式新增“x 小时后自动关灯”命令和对应 ID。

## 来源与许可说明

本仓库中的灯光控制、命令路由和 MQTT 桥接代码为本项目重新编写。ESP-SR 是乐鑫官方提供的语音识别框架，按其许可证和组件管理器要求使用；本仓库不复制 ESP-SR 模型或第三方示例源码。

技术参考：

- [Espressif ESP-SR](https://github.com/espressif/esp-sr)
- [ESP32 MultiNet Voice Demo](https://github.com/nazirlouis/esp32-multinet-demo)
- [Kavach](https://github.com/ashishjoy19/Kavach)
