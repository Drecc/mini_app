# todo list
1. 

# mini_app
一个简单的chromium demo

## 快速开始

1. 拉取chromium代码,切换到80分支
2. 修改.gclient文件，加入以下内容，并执行gclient sync

```
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
