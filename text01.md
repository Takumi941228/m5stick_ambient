# 開発環境の構築

## M5Stick-C-Plusのボード定義のインストール

Arduino IDEでM5Stick-C-Plusの開発するための環境構築として、ESP32のボード定義のインストールとM5Stick-C-PLUS用のライブラリのインストールが必要となる。

Arduino IDEのメニュー[ファイル] → [環境設定] → [追加のボードマネージャーのURL:]
* https://m5stack.oss-cn-shenzhen.aliyuncs.com/resource/arduino/package_m5stack_index.json

次に[ツール] → [ボード] → [ボードマネージャー]でボードマネージャーを開き、検索バーに「M5Stack」と入力し、
「M5Stack by M5Stack official」をインストールする。

### ボードの選択
[ツール] → [ボード] → [M5Stack Arduino] → [M5Stick-C-Plus]を選択する。

## M5Stick-C-Plus用ライブラリのインストール

M5Stick-C-Plusには液晶画面、ボタン制御、ブザーなど便利な機能が備わっています。これらの機能を活用するのに
専用のライブラリと利用すると簡単になります。

Arduino IDEのメニュー[スケッチ] → [ライブラリをインクルード] → [ライブラリを管理]でライブラリマネージャーを開き、検索バーに「m5stickcplus」と入力し、「M5StickCPlus by M5Stack official」をInstall allでインストールする。

## USBシリアルドライバのインストール

Arduino IDEでの開発にはUSBシリアルドライバのインストールが必要です。USBシリアルドライバはM5Stackの公式Webサイトにてダウンロードが可能です。

* https://m5stack.com/pages/download

windows版のCP2104 Driverをzipファイルにてダウンロードし、解凍後にインストール作業を行ってください。