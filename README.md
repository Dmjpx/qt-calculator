# 计算器

使用 **C++17 + Qt Widgets** 实现，窗口标题和程序文件名均为“计算器”。按钮按照题目参考图排列。

## 直接运行

双击 `dist/计算器.exe`。分发或拷贝到其他 Windows 64 位电脑时，请复制整个 `dist` 文件夹，保留 DLL 和 `platforms` 子目录。

## 功能

- 支持 `+`、`-`、`*`、`/`，小数及负数结果。
- 输入时显示当前算式，例如 `12 +`、`12 + 3`，按 `=` 后显示结果 `15`。继续运算时显示中间结果与下一个运算符，例如 `2 + 3 *` 显示为 `5 *`。
- `CE` 清空所有输入和待执行运算，回到 `0`。
- `<-` 删除当前输入的最后一位，删空后显示 `0`。
- 刚输入运算符、尚未输入第二个数时，`<-` 可撤销该运算符。
- `=` 显示结果；结果可继续参与运算，直接输入数字则开始新运算。
- 连续运算按普通计算器的输入顺序执行，例如 `2 + 3 * 4 =` 得到 `20`。
- 连续按运算符会替换待执行的运算符。
- 除数为零时显示中文提示，按 CE、退格或输入数字可恢复。
- 键盘支持数字、小数点和四则运算符；Enter 或 `=` 计算，Backspace 删除，Esc 或 Delete 清零。
- 输入最多 16 个字符，结果以 15 位有效数字显示，使用浮点数计算。

## 在 Visual Studio / VS Code 中打开

在 Visual Studio 中选择“打开文件夹”并打开本目录，使用 `qt-release` CMake 配置，启动目标选择 `calculator`。也可在 VS Code 中通过 CMake Tools 扩展选择该预设。

当前预设匹配本机安装路径：Qt `D:/Qt/6.11.1/llvm-mingw_64`，编译器 `D:/Qt/Tools/llvm-mingw1706_64`。更换机器时需调整 `CMakePresets.json` 中的路径。

在 VS Code 中安装 Microsoft 的 **C/C++** 扩展（`ms-vscode.cpptools`），通过“文件 → 打开文件夹”打开本目录。进入左侧“运行和调试”，选择“调试计算器（C++ 窗口程序）”，按 **Ctrl+F5** 运行；启动前会自动进行 Debug 构建并整理 DLL。也可以按 **F5** 使用本机 LLVM 的 LLDB 调试器启动并设置源码断点。按 **Ctrl+Shift+B** 可以只执行构建任务。

如果日志出现 `cmake.exe --debugger`，调试的是 CMake 配置脚本。请停止该调试，按 `Ctrl+Shift+D`，选择上述 C++ 窗口程序配置，再按 F5。计算器在独立窗口中显示；若断点停在 `window.show()` 之前，按 F5 继续后窗口才会出现。本机 CMake 重复配置曾异常退出，已在编辑器配置中加入 `--fresh` 重新生成缓存；若 CMake Tools 仍无法配置，可使用上述构建任务。

不要使用仅编译当前文件的“运行 C/C++ 文件”或 Code Runner 的“Run Code”来启动本项目：Qt 工程需要同时编译多个 `.cpp` 文件，并链接 Qt 库。现有 `dist/计算器.exe` 也可以直接双击运行。

编辑器的 Qt 头文件识别由 `.vscode/c_cpp_properties.json` 配置，优先读取构建生成的 `build/compile_commands.json`，使用与实际编译相同的头文件路径、宏和编译参数。如果出现 `C/C++(1696)`（无法打开 `QApplication`、`QWidget`），先按 `Ctrl+Shift+B` 构建，再在命令面板执行 `C/C++: Reset IntelliSense Database`；必要时执行 `Developer: Reload Window`。更换 Qt 安装位置时，还需要同步修改此配置中的路径。

也可以使用 Qt Creator 打开 `CMakeLists.txt`，选择匹配的 Qt 6 桌面套件。工程同时兼容 MSVC，使用 MSVC 时应搭配对应的 MSVC Qt 套件。

## 一键构建和整理运行文件

在本目录 PowerShell 终端执行：

```powershell
powershell -ExecutionPolicy Bypass -File .\build.ps1
```

脚本使用本机 Qt 安装目录，不会下载或安装软件。Qt 位于其他盘符时，可通过 `-QtRoot` 指定安装根目录。

## 源码结构

- `main.cpp`：程序入口。
- `calculatorwindow.h/.cpp`：Qt 窗口、布局、鼠标及键盘交互。
- `calculatorengine.h/.cpp`：运算、删除、清零及错误处理。
- `CMakeLists.txt`、`CMakePresets.json`：工程与本机构建配置。
- `preview.png`：实际 Windows 界面预览（窗口内容区域）。

