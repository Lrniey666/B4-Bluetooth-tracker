# 協定

APP 與韌體只交換一行 UTF-8 文字，經 Classic Bluetooth SPP（`BluetoothSerial`）。沒有長度欄、沒有校驗、沒有 JSON。

比對在韌體是 `DATA == "……"`。空白、換行或連點過快造成的黏包，都會對不上——當年筆記已寫：點太快會亂碼，要隔幾秒再送下一筆。

## 字串表 / Command table

| APP 送出 Sent | 韌體動作 Firmware | 備註 |
| --- | --- | --- |
| `開燈` | `GPIO22 LOW` | 低態點亮 |
| `關燈` | `GPIO22 HIGH` | |
| `播放小星星` | `star()` | 課堂示範曲 |
| `播放小蜜蜂` | `littleBee()` | 課堂示範曲 |
| `播放天空之城` | `sky()` | 課堂示範曲 |
| `播放極樂淨土` | `GJ()` | 課堂示範曲 |
| `播放Pekora bgm` | `Pekora()` | 課堂示範曲 |
| `停止播放` | `stp()` | 也可按 GPIO12 |
| `vi6` | duty 基準 100 | 最大 |
| `vi5` | 60 | |
| `vi4` | 20 | |
| `vi3` | 15 | |
| `vi2` | 10 | |
| `vi1` | 5 | |
| `vi0` | 0 | 靜音 |

裝置名稱：`追蹤器1`。`setup()` 會把 Classic Bluetooth 位址印到 115200。

## APP 畫面

`app/B4-tracker.aia`（匯入後專案名 `esp_teat10_1`，2021-09-14）：

| 畫面 | 用途 |
| --- | --- |
| Screen1 | 標題「ESP32 test10」→ 進入測試畫面 |
| Screen2 | `ListPicker` 配對、送上表指令 |
| test | 音量圖示 Canvas 的另一份測試 |

課堂「猶戲」已從成品拆出，完整備份在本機 `local/minigame/`（gitignore）。

打包圖示為 `T10.png`。課堂手機桌面顯示「B4藍芽追蹤 Beta版」。

## 音量怎麼算

譜上每個音有 `*dutyCycle[]`（多半是 300）。實際寫入 LEDC 的是：

```text
volume = (noteDuty / 100) * btdutyCycle
```

`btdutyCycle` 只在收到 `vi*` 時改。播放迴圈裡仍可再收一筆新指令：先 `volume()` 處理燈與音量；若是換曲則 `dost()`。
