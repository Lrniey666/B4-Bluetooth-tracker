# 貢獻指南

語言：[繁體中文](CONTRIBUTING.md) · [English](docs/CONTRIBUTING.en.md)

這是 2021 高職專題的封存 Showcase Repository。歡迎修文件、補編譯註記、加**自製**提示音。請先當歷史文物看，再動手。

## 動工前

1. 讀根目錄 [`README.md`](README.md) 與 [`docs/README.md`](docs/README.md)。
2. 改腳位或指令時，同步 [`docs/hardware.md`](docs/hardware.md)、[`docs/protocol.md`](docs/protocol.md) 與 `firmware/BT9/BT9.ino`。
3. 衝突時：**2021 繳交行為 > Showcase Repository 文件**。文件寫錯就改文件，不要為了「比較現代」重寫 BT9 邏輯。

## 慣例

| 項目 | 約定 |
| --- | --- |
| 對外說明 | 繁中在 `README.md`；英文在 `docs/README.en.md`，兩邊一起改 |
| 日期 | `YYYY-MM-DD`，台北時間 |
| 變更紀錄 | `CHANGELOG.md` 的 `## [Unreleased]`（Keep a Changelog 2.0.0） |
| 換行 | LF（`.gitattributes`） |

## 請不要

- 提交 `original-data/`、`local/`、影片、`.pds`、問卷 csv／xlsx、自由建議原文
- 提交能辨識組員或受訪者臉的照片
- 把知名旋律的完整曲譜、BGM 檔再加進來
- 在程式裡硬寫本機絕對路徑或帳號
- 為了「順眼」重構 TUNIOT 產生碼卻聲稱這是 2021 原貌

不可逆的動作（force push、刪遠端、把原料庫打進歷史）請先問。

## 改完必做

1. Notable 變更寫進 `CHANGELOG.md` → `## [Unreleased]`
2. 動到 Hero／安裝／結構 → 繁中與英文 README 一起改
3. 若改了 `BT9.ino`，在 [`docs/firmware.md`](docs/firmware.md) 註明「Showcase Repository 偏離繳交檔」的那幾行
