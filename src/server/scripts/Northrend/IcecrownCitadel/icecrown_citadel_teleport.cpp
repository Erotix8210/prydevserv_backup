/*
 * Copyright (C) 2008-2010 TrinityCore <http://www.trinitycore.org/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "ScriptPCH.h"
#include "icecrown_citadel.h"
#include "Spell.h"

enum eSpellAction
{
    LIGHT_S_HAMMER_TELEPORT         = 70781,
    ORATORY_OF_THE_DAMNED_TELEPORT  = 70856,
    RAMPART_OF_SKULLS_TELEPORT      = 70857,
    DEATHBRINGER_S_RISE_TELEPORT    = 70858,
    UPPER_SPIRE_TELEPORT            = 70859,
    FROZEN_THRONE_TELEPORT          = 70860,
    SINDRAGOSA_S_LAIR_TELEPORT      = 70861
};

#define GOSSIP_SENDER_ICC_PORT 631

class icecrown_citadel_teleport : public GameObjectScript
{
    public:
        icecrown_citadel_teleport() : GameObjectScript("icecrown_citadel_teleport") { }

        bool OnGossipHello(Player* player, GameObject* go)
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Teleport to Light's Hammer.", GOSSIP_SENDER_ICC_PORT, LIGHT_S_HAMMER_TELEPORT);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Teleport to The Spire.", GOSSIP_SENDER_ICC_PORT, ORATORY_OF_THE_DAMNED_TELEPORT);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Teleport to Rampart of Skulls.", GOSSIP_SENDER_ICC_PORT, RAMPART_OF_SKULLS_TELEPORT);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Teleport to Deathbringer's Rise.", GOSSIP_SENDER_ICC_PORT, DEATHBRINGER_S_RISE_TELEPORT);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Teleport to Icecrown Citadel.", GOSSIP_SENDER_ICC_PORT, UPPER_SPIRE_TELEPORT);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Teleport to the Frost Queen's Lair.", GOSSIP_SENDER_ICC_PORT, SINDRAGOSA_S_LAIR_TELEPORT); 
            if (InstanceScript* pInstance = go->GetInstanceScript())
            {
                if (pInstance->GetBossState(DATA_MARROWGAR_EVENT) == DONE)
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Teleport to the Oratory of the Damned.", GOSSIP_SENDER_ICC_PORT, ORATORY_OF_THE_DAMNED_TELEPORT);
                if (pInstance->GetBossState(DATA_DEATHWHISPER_EVENT) == DONE)
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Teleport to the Rampart of Skulls.", GOSSIP_SENDER_ICC_PORT, RAMPART_OF_SKULLS_TELEPORT);
                if (pInstance->GetBossState(DATA_DEATHWHISPER_EVENT) == DONE)
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Teleport to the Deathbringer's Rise.", GOSSIP_SENDER_ICC_PORT, DEATHBRINGER_S_RISE_TELEPORT);
                if (pInstance->GetBossState(DATA_SAURFANG_EVENT) == DONE)
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Teleport to the Upper Spire.", GOSSIP_SENDER_ICC_PORT, UPPER_SPIRE_TELEPORT);
                if (pInstance->GetBossState(DATA_VALITHRIA_DREAMWALKER) == DONE)
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Teleport to the Sindragosa's Lair", GOSSIP_SENDER_ICC_PORT, SINDRAGOSA_S_LAIR_TELEPORT);
                if (pInstance->GetBossState(DATA_PROFESSOR_PUTRICIDE) == DONE && pInstance->GetBossState(DATA_BLOOD_QUEEN_LANATHEL_EVENT) == DONE && pInstance->GetBossState(DATA_SINDRAGOSA) == DONE)
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Teleport to The Frozen Throne", GOSSIP_SENDER_ICC_PORT, FROZEN_THRONE_TELEPORT);
            }

            player->SEND_GOSSIP_MENU(go->GetGOInfo()->GetGossipMenuId(), go->GetGUID());
            return true;
        }

        bool OnGossipSelect(Player* player, GameObject* /*go*/, uint32 sender, uint32 action)
        {
            player->PlayerTalkClass->ClearMenus();
            player->CLOSE_GOSSIP_MENU();
            SpellEntry const* spell = sSpellStore.LookupEntry(action);
            if (!spell)
                return false;

            if (player->isInCombat())
            {
                Spell::SendCastResult(player, spell, 0, SPELL_FAILED_AFFECTING_COMBAT);
                return true;
            }

            if (sender == GOSSIP_SENDER_ICC_PORT)
                player->CastSpell(player, spell, true);

            return true;
        }
};

void AddSC_icecrown_citadel_teleport()
{
    new icecrown_citadel_teleport();
}
