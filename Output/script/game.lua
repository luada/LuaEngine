--[[Created by Luada
全局游戏状态管理类
--]]

require "debugger"
require "player"

--------------------------------------------------------------------------------
autoImpScriptClass("BufferReader", "ScriptObject", Demo.CDataReader)
autoImpScriptClass("BufferWriter", "ScriptObject", Demo.CDataWriter)
--------------------------------------------------------------------------------

Game = class(Singleton, ScriptObject)
autoWrapEngine("Game", "engineObj_", Demo.CGame)

local prop = Property(Game)


--------------------------------------------------------------------------------
function Game:__init()
	--[[
	构造
	--]]
	ScriptObject.__init(self)
	local this = prop[self]
	this._debugger = Debugger()
end

function Game:__release()
	--[[
	析构
	--]]
	local this = prop[self]
	this:clear()
end

function Game:__onLoaded()
	--[[
	对象被引擎加载数据后的回调函数
	--]]
	local this = prop[self]
end

function Game.New()
	--[[
	static method:此函数被底层回调用
	--]]
	return game
end

function Game:clear()
	--[[
	清理操作
	--]]
	local this = prop[self]
end

function Game:init()
	--[[
	初始化
	--]]
	local this = prop[self]
	this._debugger:onDebug("/s")
	this._player = Player("DemoPlayer")
	return true
end

function Game:onUpdate()
	--[[
	更新tick操作
	--]]
	local this = prop[self]
	this._player:tick()
	---print("Game:onUpdate")
end

function Game:onDbgCmd(cmd)
	--[[
	调试命令回调
	@param	cmd: 脚本类名
	@type	cmd: string
	--]]
	local this = prop[self]
	print("Game:onDbgCmd", cmd)
	this._debugger:onDebug(cmd)
end

function Game:getScriptClass()
	--[[
	获取类的名称
	--]]
	return "Game"
end

game = Game()
