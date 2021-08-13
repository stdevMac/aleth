#pragma once

#include "Account.h"
#include "BlockDetails.h"
#include "BlockQueue.h"
#include "ChainParams.h"
#include "DatabasePaths.h"
#include "LastBlockHashesFace.h"
#include "State.h"
#include "Transaction.h"
#include "VerifiedBlock.h"
#include <libdevcore/Exceptions.h>
#include <libdevcore/Guards.h>
#include <libdevcore/Log.h>
#include <libdevcore/db.h>
#include <libethcore/BlockHeader.h>
#include <libethcore/Common.h>
#include <libethcore/SealEngine.h>
#include <boost/filesystem/path.hpp>
#include <chrono>
#include <deque>
#include <unordered_map>
#include <unordered_set>

namespace dev
{
class OverlayDB;

namespace eth
{
class State;
class Block;
class ImportPerformanceLogger;

using BlocksHash = std::unordered_map<h256, bytes>;
using TransactionHashes = h256s;
using UncleHashes = h256s;

using ProgressCallback = std::function<void(unsigned, unsigned)>;

/**
 * @brief Implements the blockchain database. All data this gives is disk-backed.
 * @threadsafe
 */

class BlockChainWrapper
{
public:
    BlockChainWrapper(OverlayDB);
    ~BlockChainWrapper();

    LastBlockHashesFace const& lastBlockHashes() const { return *m_lastBlockHashes; }

    /// Get all blocks not allowed as uncles given a parent (i.e. featured as uncles/main in parent,
    /// parent + 1, ... parent + @a _generations).
    /// @returns set including the header-hash of every parent (including @a _parent) up to and
    /// including generation + @a _generations together with all their quoted uncles.
    h256Hash allKinFrom(h256 const&, unsigned) const;

    bool isKnown(h256 const&);
    bytes block(h256 const&) const;
    BlockDetails parentHashOfBlock(h256 const&) const;
    BlockDetails parentHashOfBlock() const { return parentHashOfBlock(the); }

private:
    OverlayDB state;
    std::unique_ptr<LastBlockHashesFace> m_lastBlockHashes;
    h256 the;
};
}  // namespace eth
}  // namespace dev