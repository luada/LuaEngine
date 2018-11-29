--[[Created by Luada
���ܷ�����ص�ģ��
--]]

local enterLoadModule = {}	---��¼����ģ����ص�ʱ��, map[module] = {currentLoadCount, currentTime}
local leaveLoadModule = {}	---��¼�˳�ģ����ص�ʱ�̣�map[module] = {currentLoadCount, currentTime}
local enterLoadIdx = {}		---��¼����ģ����ص�������map[currentLoadCount] = module

local currentLoadCount = 0
local invalidElapse = -1
local lastRecordCount = 0

local disableRecord = true


function OnEnterRecordRequire(module)
	--[[
	����ģ����ص�����ص�
	@param	module��
	@type	module��string
	--]]
	if disableRecord then
		return
	end
	currentLoadCount = currentLoadCount + 1
	enterLoadModule[module] = {currentLoadCount, YXS_PVE.GetElapse()}
	enterLoadIdx[currentLoadCount] = module
end

function OnLeaveRecordRequire(module)
	--[[
	�˳�ģ����ص�����ص�
	@param	module��
	@type	module��string
	--]]
	if disableRecord then
		return
	end
	leaveLoadModule[module] = {currentLoadCount, YXS_PVE.GetElapse()}
end

local function fetchOneRequire(recordOut, module)
	--[[
	��ȡһ��ģ��ļ���ʱ������
	@param	recordOut: ��¼��������ģ�����ĵı�
	@type	recordOut: table
	@param	module: �����ҵ�ģ��
	@type	module��string
	@param	return������ģ��module��ʱ������
	@ype	return��float
	--]]
	local elapse = recordOut[module]
	if elapse ~= nil then
		return elapse
	end
	local idx0, elapse0 = unpack(enterLoadModule[module] or {})
	local idx1, elapse1 = unpack(leaveLoadModule[module] or {})
	if idx0 == nil or idx1 == nil then
		recordOut[module] = invalidElapse
		return invalidElapse
	end
	elapse = elapse1 - elapse0
	for i = idx0 + 1, idx1 do
		local curModule = enterLoadIdx[i]
		local curElapse = fetchOneRequire(recordOut, curModule)
		if curElapse == invalidElapse then
			elapse = invalidElapse
			break
		end
		elapse = elapse - curElapse
	end
	recordOut[module] = elapse
	return elapse
end

function dumpRecordRequire(fromIdx, title)
	--[[
	���������fromIdx��ʼ��ģ����ص��������
	@param	fromIdx: ����
	@type	fromIdx: int
	@param	title: ���μ�¼�ı���
	@type	title: string
	--]]
	if disableRecord then
		return
	end
	local recordOut = {}
	for k, _ in pairs(leaveLoadModule) do
		fetchOneRequire(recordOut, k)
	end
	
	lastRecordCount = currentLoadCount
	local sumElapse = 0
	local file = io.open("LuaProfile.txt", "a")
	file:write(string.format("\n*-----------%s-----------*\n", title))
	
	local sortElapses = {}
	for i = fromIdx, currentLoadCount do
		local module = enterLoadIdx[i]
		if module ~= nil then
			local curElapse = recordOut[module] or invalidElapse
			if curElapse ~= invalidElapse then
				sumElapse = sumElapse + curElapse
			end
			sortElapses[module] = curElapse
		end
	end
	
	for k, v in table.sortIterator(sortElapses, function(a, b) return sortElapses[a] > sortElapses[b] end) do
	    local idx = enterLoadModule[k][1]
	    local info = string.format("%03d:[%f(seconds)] %s\n", idx, v, k)
	    file:write(info)
	end
	
	file:write(string.format("\n==total file:[%d] require modules elapse:[%f(seconds)], time:[%f(seconds)]=====\n\n", currentLoadCount - fromIdx + 1, sumElapse, YXS_PVE.GetElapse()))
	file:close()
end

function getCurrentLoadCount()
	--[[
	��ȡ��ǰ�Ѿ�����ģ�������
	@param	return:
	@type	return: int
	--]]
	return currentLoadCount
end

function getLastRecordCount()
	--[[
	��ȡ��һ�α����¼������
	--]]
	return lastRecordCount
end



--------------------------------------------------------------------------------
local ProfileFuncElapse = {}	---table {functionName = elapse}
local CallStackFuncElapse = {0}
local ProfileInnerCall = false
local SkipSubCallElapse = true
local UnknownFnName = "???"
local CallStackFuncName = {UnknownFnName}
local YieldFuncName = "yield"
local ProfileRecElapse = 0


GamePauseTime = 0
StackFrameCount = 0
StackFramePauseElases = {}

local function onDbgHookProfileTrace(event)
	--[[
	��Ӧ���ܷ����Ĺ��Ӻ���
	@param	event: describe the event that has triggered its call: "call", 
					"return" (or "tail return"), "line", and "count". 
	@param	event: string
	--]]
	if ProfileInnerCall then
		return
	end

	local nowTime = YXS_PVE.GetElapse()
	ProfileInnerCall = true
	
	if event == "call" then
		local dbgInfo = debug.getinfo(2, "nS")
		local funcName = dbgInfo.name or UnknownFnName
		local linedefined = dbgInfo.linedefined
		local fileName = string.sub(dbgInfo.short_src, 10, -7)

		local fullName = funcName
		if fileName ~= "" then
			if linedefined ~= -1 then
				fullName = string.format("%s(%d):%s", fileName, linedefined, funcName)
			else
				fullName = string.format("%s:%s", fileName, funcName)
			end
		end
		
		local item = ProfileFuncElapse[fullName]
		if item == nil then
			item = {
				fnName = fullName,
				callTimes = {},
				elapse = 0,
				maxElapse = 0,
				totalElapse = 0,
				count = 0,
			}
			ProfileFuncElapse[fullName] = item
		end
		table.insert(item.callTimes, nowTime)
		StackFrameCount = StackFrameCount + 1
		StackFramePauseElases[StackFrameCount] = 0
		table.insert(CallStackFuncName, fullName)
		table.insert(CallStackFuncElapse, 0)
		if fullName == YieldFuncName then
			GamePauseTime = nowTime
		end
	else
		local fnName = CallStackFuncName[#CallStackFuncName]
		local item = ProfileFuncElapse[fnName]
		if item == nil then
			FATAL_MSG("ProfileTrace Can not Find Item!", fnName)
		else
			if fnName == YieldFuncName then
				calStackGamePauseElapse(nowTime - GamePauseTime, StackFrameCount)
			end
			
			local callTime = item.callTimes[#item.callTimes]
			table.remove(item.callTimes)
			local selfElapse = nowTime - callTime - StackFramePauseElases[StackFrameCount]

			if fnName == YieldFuncName then	----����YieldFuncName
				selfElapse = 0
			elseif selfElapse < 0 then
				FATAL_MSG("selfElapse < 0", nowTime - callTime, StackFramePauseElases[StackFrameCount], fnName, StackFrameCount)
			end

			local subElapse = CallStackFuncElapse[#CallStackFuncElapse]
			table.remove(CallStackFuncElapse)
			table.remove(CallStackFuncName)

			CallStackFuncElapse[#CallStackFuncElapse] = CallStackFuncElapse[#CallStackFuncElapse] + selfElapse
			
			item.elapse = SkipSubCallElapse and selfElapse or (selfElapse - subElapse)
			item.maxElapse = math.max(item.elapse, item.maxElapse)
			item.totalElapse = item.totalElapse + item.elapse
			item.count = item.count + 1
			StackFrameCount = StackFrameCount - 1
		end
	end
	---calStackGamePauseElapse(YXS_PVE.GetElapse() - nowTime, StackFrameCount)	
	ProfileInnerCall = false
end

local function writeProfileResult(orders, lastName)
	--[[
	��¼��־�����ṹ
	@param	orders: ���кõı�
	@type	orders: table
	@param	lastName: �����ļ���
	@param	lastName: string
	--]]
	local dat = os.date("*t")
	local path = string.format("../Y_GameLog/Ver_%d/%s/%s/%s/%s", getResVersion(), dat.year, dat.month, dat.day, dat.hour)
	createDir(path)
	local file = io.open(string.format("%s/%s_Profile_%s.txt", path, game:GetGameTableID(), lastName),"wb")
	if file == nil then
		return
	end
	file:write(string.format("funcName	maxElapse	totalElapse(%f)	CallCount	average", ProfileRecElapse))
	for _, v in ipairs(orders) do
		local funcName = v[1]
		local v = ProfileFuncElapse[funcName]
		local info = string.format("\n%s\t%f\t%f\t%d\t%f", 
				funcName, v.maxElapse * 1000, v.totalElapse * 1000, v.count, (v.totalElapse / v.count) * 1000)
		file:write(info)
	end
	file:close()
end

function calStackGamePauseElapse(elapse, stackTop)
	--[[
	����Э��ջ����Ϸ��ͣ��ʱ������
	@param	elapse:
	@type	elapse: number
	@param	stackTop: ջ��֡��
	@type	stackTop��int
	--]]
	for i = 0, stackTop do
		StackFramePauseElases[i] = elapse + (StackFramePauseElases[i] or 0)
	end
end

function startProfile(skipSubCallElapse)
	--[[
	��ʼ���ܷ���
	@param	skipSubCallElapse: �Ƿ������Ӻ����ĵ�������
	@type	skipSubCallElapse: boolean
	--]]
	ProfileFuncElapse = {}
	CallStackFuncElapse = {0}
	CallStackFuncName = {UnknownFnName}
	GamePauseTime = 0
	StackFrameCount = 0
	StackFramePauseElases = {}
	SkipSubCallElapse = skipSubCallElapse
	ProfileRecElapse = YXS_PVE.GetElapse()

	local co = game:getCoProcess()
	local handle = co and co:handle() or nil
	debug.sethook(handle, onDbgHookProfileTrace, "cr")
end


function stopProfile()
	--[[
	ֹͣ���ܷ����������ͳ�ƽ��
	--]]
	debug.sethook()
	ProfileRecElapse = YXS_PVE.GetElapse() - ProfileRecElapse

	local orders = {}
	for k, v in pairs(ProfileFuncElapse) do
		if v.count > 0 and v.fnName ~= UnknownFnName and v.fnName ~= YieldFuncName then
			table.insert(orders, {k, v.totalElapse, v.totalElapse / v.count})
		end
	end

	local function sortTotalElapse(item1, item2)
		return item1[2] > item2[2]
	end
	table.sort(orders, sortTotalElapse)
	writeProfileResult(orders, "totalElapse")

	local function sortAverage(item1, item2)
		return item1[3] > item2[3]
	end
	table.sort(orders, sortAverage)
	writeProfileResult(orders, "average")
end
