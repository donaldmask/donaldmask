# Libraries

| Name                     | Description |
|--------------------------|-------------|
| *libdonaldmask_cli*         | RPC client functionality used by *donaldmask-cli* executable |
| *libdonaldmask_common*      | Home for common functionality shared by different executables and libraries. Similar to *libdonaldmask_util*, but higher-level (see [Dependencies](#dependencies)). |
| *libdonaldmask_consensus*   | Stable, backwards-compatible consensus functionality used by *libdonaldmask_node* and *libdonaldmask_wallet* and also exposed as a [shared library](../shared-libraries.md). |
| *libdonaldmaskconsensus*    | Shared library build of static *libdonaldmask_consensus* library |
| *libdonaldmask_kernel*      | Consensus engine and support library used for validation by *libdonaldmask_node* and also exposed as a [shared library](../shared-libraries.md). |
| *libdonaldmaskqt*           | GUI functionality used by *donaldmask-qt* and *donaldmask-gui* executables |
| *libdonaldmask_ipc*         | IPC functionality used by *donaldmask-node*, *donaldmask-wallet*, *donaldmask-gui* executables to communicate when [`--enable-multiprocess`](multiprocess.md) is used. |
| *libdonaldmask_node*        | P2P and RPC server functionality used by *donaldmaskd* and *donaldmask-qt* executables. |
| *libdonaldmask_util*        | Home for common functionality shared by different executables and libraries. Similar to *libdonaldmask_common*, but lower-level (see [Dependencies](#dependencies)). |
| *libdonaldmask_wallet*      | Wallet functionality used by *donaldmaskd* and *donaldmask-wallet* executables. |
| *libdonaldmask_wallet_tool* | Lower-level wallet functionality used by *donaldmask-wallet* executable. |
| *libdonaldmask_zmq*         | [ZeroMQ](../zmq.md) functionality used by *donaldmaskd* and *donaldmask-qt* executables. |

## Conventions

- Most libraries are internal libraries and have APIs which are completely unstable! There are few or no restrictions on backwards compatibility or rules about external dependencies. Exceptions are *libdonaldmask_consensus* and *libdonaldmask_kernel* which have external interfaces documented at [../shared-libraries.md](../shared-libraries.md).

- Generally each library should have a corresponding source directory and namespace. Source code organization is a work in progress, so it is true that some namespaces are applied inconsistently, and if you look at [`libdonaldmask_*_SOURCES`](../../src/Makefile.am) lists you can see that many libraries pull in files from outside their source directory. But when working with libraries, it is good to follow a consistent pattern like:

  - *libdonaldmask_node* code lives in `src/node/` in the `node::` namespace
  - *libdonaldmask_wallet* code lives in `src/wallet/` in the `wallet::` namespace
  - *libdonaldmask_ipc* code lives in `src/ipc/` in the `ipc::` namespace
  - *libdonaldmask_util* code lives in `src/util/` in the `util::` namespace
  - *libdonaldmask_consensus* code lives in `src/consensus/` in the `Consensus::` namespace

## Dependencies

- Libraries should minimize what other libraries they depend on, and only reference symbols following the arrows shown in the dependency graph below:

<table><tr><td>

```mermaid

%%{ init : { "flowchart" : { "curve" : "basis" }}}%%

graph TD;

donaldmask-cli[donaldmask-cli]-->libdonaldmask_cli;

donaldmaskd[donaldmaskd]-->libdonaldmask_node;
donaldmaskd[donaldmaskd]-->libdonaldmask_wallet;

donaldmask-qt[donaldmask-qt]-->libdonaldmask_node;
donaldmask-qt[donaldmask-qt]-->libdonaldmaskqt;
donaldmask-qt[donaldmask-qt]-->libdonaldmask_wallet;

donaldmask-wallet[donaldmask-wallet]-->libdonaldmask_wallet;
donaldmask-wallet[donaldmask-wallet]-->libdonaldmask_wallet_tool;

libdonaldmask_cli-->libdonaldmask_util;
libdonaldmask_cli-->libdonaldmask_common;

libdonaldmask_common-->libdonaldmask_consensus;
libdonaldmask_common-->libdonaldmask_util;

libdonaldmask_kernel-->libdonaldmask_consensus;
libdonaldmask_kernel-->libdonaldmask_util;

libdonaldmask_node-->libdonaldmask_consensus;
libdonaldmask_node-->libdonaldmask_kernel;
libdonaldmask_node-->libdonaldmask_common;
libdonaldmask_node-->libdonaldmask_util;

libdonaldmaskqt-->libdonaldmask_common;
libdonaldmaskqt-->libdonaldmask_util;

libdonaldmask_wallet-->libdonaldmask_common;
libdonaldmask_wallet-->libdonaldmask_util;

libdonaldmask_wallet_tool-->libdonaldmask_wallet;
libdonaldmask_wallet_tool-->libdonaldmask_util;

classDef bold stroke-width:2px, font-weight:bold, font-size: smaller;
class donaldmask-qt,donaldmaskd,donaldmask-cli,donaldmask-wallet bold
```
</td></tr><tr><td>

**Dependency graph**. Arrows show linker symbol dependencies. *Consensus* lib depends on nothing. *Util* lib is depended on by everything. *Kernel* lib depends only on consensus and util.

</td></tr></table>

- The graph shows what _linker symbols_ (functions and variables) from each library other libraries can call and reference directly, but it is not a call graph. For example, there is no arrow connecting *libdonaldmask_wallet* and *libdonaldmask_node* libraries, because these libraries are intended to be modular and not depend on each other's internal implementation details. But wallet code is still able to call node code indirectly through the `interfaces::Chain` abstract class in [`interfaces/chain.h`](../../src/interfaces/chain.h) and node code calls wallet code through the `interfaces::ChainClient` and `interfaces::Chain::Notifications` abstract classes in the same file. In general, defining abstract classes in [`src/interfaces/`](../../src/interfaces/) can be a convenient way of avoiding unwanted direct dependencies or circular dependencies between libraries.

- *libdonaldmask_consensus* should be a standalone dependency that any library can depend on, and it should not depend on any other libraries itself.

- *libdonaldmask_util* should also be a standalone dependency that any library can depend on, and it should not depend on other internal libraries.

- *libdonaldmask_common* should serve a similar function as *libdonaldmask_util* and be a place for miscellaneous code used by various daemon, GUI, and CLI applications and libraries to live. It should not depend on anything other than *libdonaldmask_util* and *libdonaldmask_consensus*. The boundary between _util_ and _common_ is a little fuzzy but historically _util_ has been used for more generic, lower-level things like parsing hex, and _common_ has been used for donaldmask-specific, higher-level things like parsing base58. The difference between util and common is mostly important because *libdonaldmask_kernel* is not supposed to depend on *libdonaldmask_common*, only *libdonaldmask_util*. In general, if it is ever unclear whether it is better to add code to *util* or *common*, it is probably better to add it to *common* unless it is very generically useful or useful particularly to include in the kernel.


- *libdonaldmask_kernel* should only depend on *libdonaldmask_util* and *libdonaldmask_consensus*.

- The only thing that should depend on *libdonaldmask_kernel* internally should be *libdonaldmask_node*. GUI and wallet libraries *libdonaldmaskqt* and *libdonaldmask_wallet* in particular should not depend on *libdonaldmask_kernel* and the unneeded functionality it would pull in, like block validation. To the extent that GUI and wallet code need scripting and signing functionality, they should be get able it from *libdonaldmask_consensus*, *libdonaldmask_common*, and *libdonaldmask_util*, instead of *libdonaldmask_kernel*.

- GUI, node, and wallet code internal implementations should all be independent of each other, and the *libdonaldmaskqt*, *libdonaldmask_node*, *libdonaldmask_wallet* libraries should never reference each other's symbols. They should only call each other through [`src/interfaces/`](`../../src/interfaces/`) abstract interfaces.

## Work in progress

- Validation code is moving from *libdonaldmask_node* to *libdonaldmask_kernel* as part of [The libdonaldmaskkernel Project #24303](https://github.com/donaldmask/donaldmask/issues/24303)
- Source code organization is discussed in general in [Library source code organization #15732](https://github.com/donaldmask/donaldmask/issues/15732)
