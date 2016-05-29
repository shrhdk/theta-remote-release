# THETA簡単リモコンの使い方

## 機能

- THETA Sのシャッターを切る
- THETA Sの動画撮影開始/停止
- Webブラウザで接続するTHETA Sを設定
- Webブラウザでファームウェアをアップデート
- USB充電

## LEDの意味

充電LED: 橙, メインLED: 緑

- 充電LED点灯: 充電中
- メインLED点滅: THETA Sに接続中
- メインLED点灯: THETA Sに接続完了/設定モード起動中
- メインLED消灯: 撮影中

## 使い方

※はじめに、THETA Sへの接続設定を行う必要があります。

THETA SとTHETAリモコンの電源を入れると自動的に接続が始まります。
THETAリモコンのメインLEDが点灯すれば接続完了です。
接続には5秒程度かかります。

ボタンを押すと静止画モードであれば、シャッターを切ります。
動画モードであれば、撮影を開始/停止します。

充電はMicro USB端子にUSB電源を接続してください。

## 接続設定

1. ボタンを押しながらリモコンの電源を入れる。
2. LEDが点滅ではなく点灯していれば、設定モードで起動している。
3. PCやスマートフォンのWi-FiでSSID:`theta-remote-release`に接続する。(パスフレーズは不要)
4. `theta-remote-release`に接続した端末のWebブラウザで`http://192.168.1.1/`にアクセスする。
5. フォームが表示されるので、THETA SのSSIDとパスワードを入力する。([参考](https://theta360.com/ja/support/manual/s/content/prepare/prepare_06.html))
6. フォームを送信して、成功ページが表示されたら、リモコンの電源を入れなおす。

## ファームウェアアップデート

最新のファームウェア (`firmware.bin`)は[リリース一覧](https://github.com/shrhdk/theta-remote-release/releases)から入手できます。

1. ボタンを押しながらリモコンの電源を入れる。
2. LEDが点滅ではなく点灯していれば、設定モードが起動している。
3. PCやスマートフォンのWi-FiでSSID:`theta-remote-release`に接続する。
4. `theta-remote-release`に接続した端末のWebブラウザで`http://192.168.1.1/update`にアクセスする。
5. フォームが表示されるので、`firmware.bin`を選択してフォームを送信する。
6. `OK`と表示されれば完了

※ファームウェアアップデート後は改めて接続設定をしてください。
