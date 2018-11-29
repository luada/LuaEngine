--[[Created by Luada
ȫ����Ϸ״̬������
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
	����
	--]]
	ScriptObject.__init(self)
	local this = prop[self]
	this._debugger = Debugger()
end

function Game:__release()
	--[[
	����
	--]]
	local this = prop[self]
	this:clear()
end

function Game:__onLoaded()
	--[[
	��������������ݺ�Ļص�����
	--]]
	local this = prop[self]
end

function Game.New()
	--[[
	static method:�˺������ײ�ص���
	--]]
	return game
end

function Game:clear()
	--[[
	�������
	--]]
	local this = prop[self]
end

function Game:init()
	--[[
	��ʼ��
	--]]
	local this = prop[self]
	this._debugger:onDebug("/s")
	this._player = Player("DemoPlayer")
	return true
end

function Game:onUpdate()
	--[[
	����tick����
	--]]
	local this = prop[self]
	this._player:tick()
	---print("Game:onUpdate")
end

function Game:onDbgCmd(cmd)
	--[[
	��������ص�
	@param	cmd: �ű�����
	@type	cmd: string
	--]]
	local this = prop[self]
	print("Game:onDbgCmd", cmd)
	this._debugger:onDebug(cmd)
end

function Game:getScriptClass()
	--[[
	��ȡ�������
	--]]
	return "Game"
end

game = Game()
