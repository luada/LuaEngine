--[[Writen by Luada

封装协程的基类，由此派生的子类需要实现protected方法: run_()

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
	获取协程句柄
	--]]
	return self._co
end

function SimpleCoroutine:resume()
	--[[
	唤醒协程
	--]]
	return coroutine.resume(self._co)
end

function SimpleCoroutine:yield()
	--[[
	挂起协程
	--]]
	return coroutine.yield(self._co)
end

function SimpleCoroutine:status()
	--[[
	返回协程的状态："running","suspended", "normal", "dead"
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
	protected方法；协程运行的处理函数，这里空处理，具体实现方法由子类来完成
	--]]
end
