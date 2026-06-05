--[[
modegameで読み込まれるlua
]]

-- 定数定義（変数で）
PAD_DN = 1
PAD_LF = 2
PAD_RI = 4
PAD_UP = 8

-- プレイヤー動作
function PLAYER(objname)
	_LOG("PLAYER()起動:"..objname.."\n")
	while true do
		local key,x,y,spd
		key = GetPad()
		x = GetObjX(objname)
		y = GetObjY(objname)
		spd = 8
		if key & PAD_LF ~= 0 then	x = x - spd	end
		if key & PAD_RI ~= 0 then	x = x + spd	end
		if key & PAD_UP ~= 0 then	y = y - spd	end
		if key & PAD_DN ~= 0 then	y = y + spd	end
		SetObjPos(objname, x, y)
		coroutine.yield()
	end
end

