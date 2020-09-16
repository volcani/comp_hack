/**
 * @file server/channel/src/packets/game/TimeLimitSync.cpp
 * @ingroup channel
 *
 * @author HACKfrost
 *
 * @brief Request from the client to sync a current instance time limit
 *  with the server time.
 *
 * This file is part of the Channel Server (channel).
 *
 * Copyright (C) 2012-2020 COMP_hack Team <compomega@tutanota.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Packets.h"

// libcomp Includes
#include <Packet.h>
#include <PacketCodes.h>
#include <ReadOnlyPacket.h>
#include <TcpConnection.h>

// object Includes
#include <MiTimeLimitData.h>
#include <Zone.h>
#include <ZoneInstance.h>

// channel Includes
#include "ChannelServer.h"

using namespace channel;

bool Parsers::TimeLimitSync::Parse(
    libcomp::ManagerPacket* pPacketManager,
    const std::shared_ptr<libcomp::TcpConnection>& connection,
    libcomp::ReadOnlyPacket& p) const {
  (void)pPacketManager;

  if (p.Size() != 0) {
    return false;
  }

  auto client = std::dynamic_pointer_cast<ChannelClientConnection>(connection);
  auto state = client->GetClientState();

  auto zone = state->GetZone();
  auto instance = zone ? zone->GetInstance() : nullptr;

  libcomp::Packet reply;
  reply.WritePacketCode(ChannelToClientPacketCode_t::PACKET_TIME_LIMIT_SYNC);

  auto timeLimitData = instance ? instance->GetTimeLimitData() : nullptr;
  if (timeLimitData) {
    float currentTime = state->ToClientTime(ChannelServer::GetServerTime());
    float expireTime = state->ToClientTime(instance->GetTimerExpire());

    reply.WriteS8(1);  // Timer exists
    reply.WriteFloat(expireTime - currentTime);
  } else {
    reply.WriteS8(0);
    reply.WriteFloat(0.f);
  }

  client->SendPacket(reply);

  return true;
}
