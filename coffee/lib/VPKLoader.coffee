ConfigManager = require("./ConfigManager")
Dialog = require("dialog")
ChildProcess = require("child_process")

module.exports =
class VPKLoader
    @instance: null
    @getInstance: () ->
        @instance = new VPKLoader() if not @instance?
        return @instance

    constructor: () ->
        @file = null
        @list = []

    load: (file) ->
        configManager = ConfigManager.getInstance()
        config = configManager.selectedConfig
        if not config? or not config.exePath? or config.exePath == ""
            Dialog.showErrorBox("No configuration", "Invalid configuration. Make sure you have a valid configuration.")
            return
        cmd = "\"#{config.exePath}\" L \"#{file}\""
        result = null
        try
            result = ChildProcess.execSync(cmd)
            result = result.toString()
        catch err
            Dialog.showErrorBox("Failed to execute vpk.exe", "Command #{cmd} failed to execute. Error output: #{err.stderr}")
            return

        @list = []
        sList = result.split("\n")
        for s in sList
            ls = s.split(" ")
            if(ls.length < 6)
                continue

            sPath = ls[0]
            sSize = ls[5].split("=")[1]
            size = parseInt(sSize)
            if isNaN(size)
                size = 0

            @list.push
                path: sPath
                size: size

        @file = file

    getDir: (path) ->
        split = path.split("/")
        if split.length < 2
            return null

        split.pop(split.length - 1)
        s = ""
        for str, i in split
            s += if i > 0 then "/#{str}" else str
        return s

    getName: (path) ->
        split = path.split("/")
        if split.length == 0
            return ""
        return split[split.length - 1]

    foldersInDir: (dir) ->
        folders = []

        for file in @list
            split = file.path.split("/")
            if dir == ""
                folders.push(file.path) if split.length < 2
                continue

            split.pop(split.length - 1)
            s = ""
            for str, i in split
                oldPath = s
                s += if i > 0 then "/#{str}" else str

                if oldPath == dir and s not in folders
                    folders.push(s)

        return folders

    filesInDir: (dir) =>
        files = []
        for file in @list
            d = @getDir(file.path)
            if d == dir or (dir == "" and not d?)
                files.push(file)
        return files