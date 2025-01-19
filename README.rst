SimpleBLE
==========

The ultimate cross-platform library and bindings for Bluetooth Low Energy (BLE).

|Latest Documentation Status|

Overview
--------

The SimpleBLE project aims to provide fully cross-platform BLE libraries and bindings
for C++, Python, Rust and other languages, designed for simplicity and ease of use,
with a consistent behavior and API across all platforms.

SimpleBLE runs on Windows 10+, macOS 10.15+, Linux (Ubuntu 20.04+), iOS 15.0+, and Android (API 31+ - Still in Alpha).

**Commercial licensing is available for projects requiring proprietary distribution.**

Below you'll find a list of frontends that are part of SimpleBLE:

* **SimpleBLE:** C++ cross-platform BLE library.
* **SimplePyBLE:** Python bindings for SimpleBLE. See the `SimplePyBLE`_ PyPI page for more details.
* **SimpleDroidBLE:** Android-specific package following the SimpleBLE API. (Still in Alpha, more to come)
* **SimpleRsBLE:** Rust bindings for SimpleBLE (LEGACY - Big refactor coming soon). See the `SimpleRsBLE`_ Crates.io page for more details.

Additionally, we also provide a set of low-level libraries that can be used to interface with the underlying Bluetooth stack on Linux:

* **SimpleBluez:** C++ abstraction layer for BlueZ over DBus.
* **SimpleDBus:** C++ wrapper for libdbus-1 with convenience classes to handle DBus object hierarchies effectively.

If you want to use SimpleBLE and need help. **Please do not hesitate to reach out!**

* Visit our `ReadTheDocs`_ page for comprehensive documentation.
* Visit our `website`_ or email us at `email us <mailto:contact@simpleble.org>`_ for commercial licensing and professional support services.
* Join our `Discord`_ server for community discussions and help.

Projects using SimpleBLE
------------------------
Don't forget to check out the following projects using SimpleBLE:

* `BrainFlow`_
* `InsideBlue`_
* `NodeWebBluetooth`_

Contributing
------------
Pull requests are welcome. For major changes, please open an issue first to discuss
what you would like to change.

License
=======

Since January 20th 2025, SimpleBLE is now available under the Business Source License 1.1 (BUSL-1.1).
This means that the project is now free to use for non-commercial purposes, but requires a commercial
license for commercial use. Each version of SimpleBLE will convert to the GNU General Public License
version 3 after four years of its initial release.

**Why purchase a commercial license?**

- Build and deploy unlimited commercial applications
- Use across your entire development team
- Zero revenue sharing or royalty payments
- Choose features that match your needs and budget
- Priority technical support included
- Clear terms for integrating into MIT-licensed projects

**You can find more information on pricing and commercial terms of service on our `website`_.**

For further enquiries, please contact us at ``contact at simpleble dot org``.

**We also offer FREE commercial licenses for small projects and early-stage companies - reach out to discuss your use case!**

Licensing FAQ
-------------

What does the Business Source License 1.1 allow and restrict?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The Business Source License allows non-production use, such as development and testing in isolated
environments. However, any use that could be considered production (including internal tools,
revenue-generating projects, or customer-facing deployments) requires purchasing a commercial license.

After four years of the initial release of a given version of SimpleBLE, the license automatically
converts to the GNU General Public License version 3.

What do you consider "commercial use"?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Commercial use in the context of SimpleBLE refers to any usage of the library or its derivatives in
a commercial setting where it contributes, either directly or indirectly, to financial gain. This
includes, but is not limited to, incorporating SimpleBLE into commercial software products, using it
in the development of services offered for a fee, or deploying it within internal tools or software
in a for-profit organization where it adds business value.

Why do I need to pay for commercial use?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Commercial licensing enables us to sustainably maintain and enhance SimpleBLE, particularly given
its growing adoption in critical sectors like medical and industrial applications. The revenue
from commercial licenses allows us to provide robust support, maintain compatibility with evolving
underlying APIs, and invest in improvements that benefit all users. This includes expanded
language support, better testing infrastructure, and more comprehensive documentation. While
SimpleBLE remains freely available for non-commercial use, the Business Source License ensures we
can continue providing the level of reliability and support that commercial applications require.

I already have a commercial license. What happens with this latest license change?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Nothing changes. You can continue using SimpleBLE under the terms of your commercial license.

I'm using SimpleBLE commercially, but don't have a commercial license. What should I do?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Please reach out at ``contact at simpleble dot org`` and we can discuss the specifics of your
situation. We have a free commercial license option for small projects and early-stage companies
and very flexible commercial licensing options for larger projects.


.. Links

.. _website: https://simpleble.org

.. _SimplePyBLE: https://pypi.org/project/simplepyble/

.. _SimpleRsBLE: https://crates.io/crates/simplersble

.. _Discord: https://discord.gg/N9HqNEcvP3

.. _ReadTheDocs: https://simpleble.readthedocs.io/en/latest/

.. |Latest Documentation Status| image:: https://readthedocs.org/projects/simpleble/badge?version=latest
   :target: http://simpleble.readthedocs.org/en/latest

.. Other projects using SimpleBLE

.. _GDSimpleBLE: https://github.com/jferdelyi/GDSimpleBLE
.. _BrainFlow: https://github.com/brainflow-dev/brainflow
.. _InsideBlue: https://github.com/eriklins/InsideBlue-BLE-Tool
.. _NodeWebBluetooth: https://github.com/thegecko/webbluetooth
