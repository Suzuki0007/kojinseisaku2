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

function DrawEnemy()
    _LOG("DrawEnemy()起動:\n")
    while true do
        local yOffset = 10     -- 描画のY軸の開始位置
        local lineHeight = 20  -- 行間
        local displayCount = 0 -- 描画した行数（これが位置を下にずらしていくカウンター）

        -- 1. 生きている敵を上から順番に表示
        for i = 0, ENEMYCOUNT - 1 do
            if IsEnemyAlive(i) then
                local hp = GetEnemyHP(i)

                -- C++のDrawFormatStringを呼び出す
                DrawEnemyStatus(10, yOffset + (displayCount * lineHeight), 255, 0, 0, "Enemy %d: HP = %d", i, hp)
                
                -- 生きている敵が表示されたら、displayCountが1増えて次の行は下にずれる
                displayCount = displayCount + 1
            else
                -- 死んでいるのに、リストにとうろくされていなｋ
            end
        end

        -- 2. 死亡した敵を【生きている敵のすぐ下】から表示
        for i = 0, ENEMYCOUNT - 1 do
            if IsEnemyAlive(i) == false then
                -- 死亡時はHPは不要なので、ダミーで 0 を渡す
                -- 文字色は見やすいように灰色（100, 100, 100）に変更しています
                DrawEnemyStatus(10, yOffset + (displayCount * lineHeight), 100, 100, 100, "Enemy %d: DEAD (HP=%d)", i, 0)
                
                -- 死亡した敵が表示されても、さらに次の行は下にずれる
                displayCount = displayCount + 1
            end
        end
        
        -- 1フレーム描画したら必ず一時中断してC++に制御を戻す
        coroutine.yield()
    end
end
