/**
 * @file server/channel/src/ZoneInstance.cpp
 * @ingroup channel
 *
 * @author HACKfrost
 *
 * @brief Represents a zone instance containing one or many zones.
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

#ifndef SERVER_CHANNEL_SRC_ZONEINSTANCE_H
#define SERVER_CHANNEL_SRC_ZONEINSTANCE_H

// channel Includes
#include "Zone.h"

// object Includes
#include <ZoneInstanceObject.h>

namespace objects {
class DestinyBox;
class InstanceAccess;
class ServerZoneInstance;
}  // namespace objects

namespace channel {

/**
 * Represents an instance of a zone instance containing one or more
 * non-global zone instances.
 */
class ZoneInstance : public objects::ZoneInstanceObject {
 public:
  /**
   * Create a new zone instance.
   * @param id Unique server ID of the zone instance
   * @param definition Pointer to the ServerZoneInstance definition
   * @param access Access definition for the instance
   */
  ZoneInstance(uint32_t id,
               const std::shared_ptr<objects::ServerZoneInstance>& definition,
               const std::shared_ptr<objects::InstanceAccess>& access);

  /**
   * Clean up the zone instance.
   */
  ~ZoneInstance();

  /**
   * Get the defintion ID of the zone instance
   * @return Defintion ID of the zone instance
   */
  uint32_t GetDefinitionID();

  /**
   * Add a zone to the instance
   * @param Pointer to the zone to add
   * @return true if the zone was successfully added, false if it was not
   */
  bool AddZone(const std::shared_ptr<Zone>& zone);

  /**
   * Get all zones in the instance
   * @return List of zones in the instance
   */
  std::list<std::shared_ptr<Zone>> GetZones();

  /**
   * Get a zone in the instance by zone ID and dynamic map ID
   * @param zoneID ID of the zone
   * @param dynamicMapID Dynamic ID of the zone
   * @return Pointer to the zone in the instance
   */
  std::shared_ptr<Zone> GetZone(uint32_t zoneID, uint32_t dynamicMapID);

  /**
   * Get a zone in the instance by its unique ID
   * @param uniqueID Unique ID of the zone in the instance
   * @return Pointer to the zone in the instance
   */
  std::shared_ptr<Zone> GetZone(uint32_t uniqueID);

  /**
   * Get all client connections in all zones in the instance
   * @return List of all client connections in the instance
   */
  std::list<std::shared_ptr<ChannelClientConnection>> GetConnections();

  /**
   * Refresh properties calculated dependent upon the players currently
   * in the instance
   */
  void RefreshPlayerState();

  /**
   * Get the state of a zone instance flag.
   * @param key Lookup key for the flag
   * @param value Output value for the flag if it exists
   * @param worldCID CID of the character the flag belongs to, 0
   *  if it affects the entire instance
   * @return true if the flag exists, false if it does not
   */
  bool GetFlagState(int32_t key, int32_t& value, int32_t worldCID);

  /**
   * Return the flag states for the zone.
   * @return Flag states for the zone.
   */
  std::unordered_map<int32_t, std::unordered_map<int32_t, int32_t>>
  GetFlagStates();

  /**
   * Get the state of a zone instance flag, returning the null default
   * if it does not exist.
   * @param key Lookup key for the flag
   * @param nullDefault Default value to return if the flag is
   *  not set
   * @param worldCID CID of the character the flag belongs to, 0
   *  if it affects the entire instance
   * @return Value of the specified flag or the nullDefault value
   *  if it does not exist
   */
  int32_t GetFlagStateValue(int32_t key, int32_t nullDefault, int32_t worldCID);

  /**
   * Set the state of a zone instance flag.
   * @param key Lookup key for the flag
   * @param value Value to set for the flag
   * @param worldCID CID of the character the flag belongs to, 0
   *  if it affects the entire instance
   */
  void SetFlagState(int32_t key, int32_t value, int32_t worldCID);

  /**
   * Get the destiny box the supplied world CID character has access
   * to which can be an individual or shared box
   * @param worldCID World CID of the character who has access to the box
   * @return Pointer to the destiny box the character has access to or
   *  null if none exists
   */
  std::shared_ptr<objects::DestinyBox> GetDestinyBox(int32_t worldCID);

  /**
   * Update the loot assigned to the destiny box assigned to a specific
   * world CID
   * @param worldCID World CID that has access to the box
   * @param add List of pointers to loot to add
   * @param remove Optional set of slots to clear before adding loot
   * @return Map of all loot that was removed upon request from the box
   *  by slot they were removed from
   */
  std::unordered_map<uint8_t, std::shared_ptr<objects::Loot>> UpdateDestinyBox(
      int32_t worldCID, uint8_t& newNext,
      const std::list<std::shared_ptr<objects::Loot>>& add,
      const std::set<uint8_t>& remove);

 private:
  /// General use flags and associated values used for event sequences etc
  /// keyed on 0 for all characters or world CID if for a specific one
  std::unordered_map<int32_t, std::unordered_map<int32_t, int32_t>> mFlagStates;

  /// Map of zones in the instance by zone ID and dynamic map ID
  std::unordered_map<uint32_t,
                     std::unordered_map<uint32_t, std::shared_ptr<Zone>>>
      mZones;

  /// Server lock for shared resources
  std::mutex mLock;
};

}  // namespace channel

#endif  // SERVER_CHANNEL_SRC_ZONEINSTANCE_H
