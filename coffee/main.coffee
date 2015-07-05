app = require("app")

WindowManager = require("./lib/WindowManager")
ConfigManager = require("./lib/ConfigManager")
MenuBar = require("./lib/MenuBar")

require("crash-reporter").start()

app.on "window-all-closed", () ->
    if process.platform != "darwin"
        app.quit()

app.on "ready", () ->
    configManager = ConfigManager.getInstance()
    configManager.load()

    if configManager.configs.length == 0
        configManager.addConfig
            name: "Team Fortress 2"
            exePath: "D:/Games/Steam/steamapps/common/Team Fortress 2/bin/vpk.exe"
        configManager.selectConfig(configManager.configs[0].id)
        configManager.save()

    windowManager = WindowManager.getInstance()
    windowManager.initialize()

    menuBar = MenuBar.getInstance()
    menuBar.initialize()

    windowManager.mainWindow.show()