## mini_app
演示如何使用content api实现一个最小electron窗口机制
 - 使用aura创建UI
 - 实现自定义js api的注入
 - 移植了微软停止维护的crosswalk的extension机制（其实也与现在chromium的extension机制原理一致）
 - 使用mojo进行browser和render的通信
 - 未引入node，导致没办法使用node丰富的第三方的库，以后有机会再补全

## 快速开始

1. 拉取chromium代码,切换到80稳定版的分支
2. 修改.gclient文件，加入以下内容，并执行gclient sync

```
//windows平台也行
target_os = "linux"
```

3. 在src目录下clone此仓库
4. 配置编译参数

```
target_cpu="x86
target_os = "linux"
root_extra_deps = ["//mini_app"]
ffmpeg_branding = "Chrome"
proprietary_codecs = true
is_debug = true
```

5. 编译前，由于代码依赖third_party/libxml，需要在visibility加入"//mini_app/core/*"依赖

6. 编译

```
ninja -C <out_dir> mini_app

```

## 运行
```
1、测试直接打开链接
<out_dir>/mini_app_executable https://www.baidu.com

2、测试打开指定格式的文件
<out_dir>/mini_app_executable mini_app/test_data/test_files/manifest.json
```
