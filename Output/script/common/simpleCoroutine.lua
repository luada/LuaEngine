--[[Writen by Luada

��װЭ�̵Ļ��࣬�ɴ�������������Ҫʵ��protected����: run_()

Example:
	GameCoroutine = class(SimpleCoroutine)

	function GameCoroutine:run_()
		local i = 0
		while true do
			print(i)
			self:yield()
		end
	end
--]]


require "base.class"

--------------------------------------------------------------------------------

SimpleCoroutine = class()

function SimpleCoroutine:__init()
	self._co = coroutine.create(self.run_)
end

function SimpleCoroutine:__release()
	if self:status() == "running" then
		self:yield()
	end
	self._co = nil
end

function SimpleCoroutine:handle()
	--[[
	��ȡЭ�̾��
	--]]
	return self._co
end

function SimpleCoroutine:resume()
	--[[
	����Э��
	--]]
	return coroutine.resume(self._co)
end

function SimpleCoroutine:yield()
	--[[
	����Э��
	--]]
	return coroutine.yield(self._co)
end

function SimpleCoroutine:status()
	--[[
	����Э�̵�״̬��"running","suspended", "normal", "dead"
	Returns the status of coroutine co, as a string: "running", if the coroutine is running 
	(that is, it called status); "suspended", if the coroutine is suspended in a call to 
	yield, or if it has not started running yet; "normal" if the coroutine is active but not 
	running (that is, it has resumed another coroutine); and "dead" if the coroutine has 
	finished its body function, or if it has stopped with an error. 
	--]]
	return coroutine.status(self._co)
end

function SimpleCoroutine:run_()
	--[[
	protected������Э�����еĴ�����������մ�������ʵ�ַ��������������
	--]]
end
