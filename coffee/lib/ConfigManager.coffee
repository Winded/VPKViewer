ipc = require("ipc")
fs = require("fs")

module.exports =
class ConfigManager
    @instance: null
    @getInstance: () ->
        @instance = new ConfigManager() if not @instance?
        return @instance

    constructor: () ->
        @folder = "#{process.env.APPDATA}/vpkviewer"
        @path =  "#{@folder}/config.json"
        @configs = []
        @selectedConfig = null

    selectConfig: (id) ->
        cfg = d for d in @configs when d.id == id
        if cfg?
            @selectedConfig = cfg
        else
            throw new Error("Config with id #{id} not found")

    load: () ->
        if not fs.existsSync(@path) or not fs.statSync(@path).isFile()
            return false
        data = fs.readFileSync(@path)
        if not data? or data == ""
            throw new Error("Failed to load config data")
        data = JSON.parse(data)
        @configs = data.configs
        @selectedConfig = data.selectedConfig
        return true

    save: () ->
        data =
            configs: @configs
            selectedConfig: @selectedConfig
        json = JSON.stringify(data)
        try
            fs.writeFileSync(@path, json)
        catch err
            fs.mkdir(@folder)
            fs.writeFileSync(@path, json)

    addConfig: (data) ->
        data.id = Math.round(Math.random() * 10000)
        # Make sure we deal with a non-remote object
        d = JSON.parse(JSON.stringify(data))
        @configs.push(d)

    removeConfig: (data) ->
        cfg = d for d in @configs when d.id == data.id
        if not cfg?
            throw new Error("Config not found")
        @configs.splice(@configs.indexOf(cfg), 1)