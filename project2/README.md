* File Structure
-------------------------------------------------------------------------------------
File                            |   Role                                            |
-------------------------------------------------------------------------------------
Product.h / Product.cpp         |   Product entity with snapshot, stock management, |
                                |   and serialization features.                     |
-------------------------------------------------------------------------------------
Cart.h / Cart.cpp:              |   Shopping cart containing CartItem struct,       |  
                                |   supporting add/remove/update operations.        |
-------------------------------------------------------------------------------------
Order.h / Order.cpp	            |   Completed order record and serialization        |
                                    management.                                     |
-------------------------------------------------------------------------------------
User.h / User.cpp	            |   Abstract base class designed for the            |
                                    authentication system.                          |
-------------------------------------------------------------------------------------
Customer.h / Customer.cpp       |	Inherits from User; includes Cart, OrderHistory,|
                                    and interaction tracking mechanisms.            |
-------------------------------------------------------------------------------------
Admin.h / Admin.cpp             |   Inherits from User; handles product management  |
                                    and statistical reporting.                      |
-------------------------------------------------------------------------------------
RecommendationEngine.h / .cpp   |   Scoring algorithm processing:                   |
                                |   views * 0.2 + purchases * 0.8 + categoryBonus   |
-------------------------------------------------------------------------------------
FileManager.h / FileManager.cpp |	Centralized file I/O operations for products,   |
                                |    users, orders, and user interactions.          |
-------------------------------------------------------------------------------------
ECommerceSystem.h / .cpp        |	Main controller managing menus, authentication, |
                                |   and orchestrating the entire system workflow.   |
-------------------------------------------------------------------------------------
main.cpp                        |	System entry point.                             |
-------------------------------------------------------------------------------------
Makefile                        |	Build system configuration.                     |
-------------------------------------------------------------------------------------

* Note:
Register and login to browse products, view/manage cart, checkout, get recommendations, see order history or use the admin account (username: admin, password: admin123) to view all products, to edit/add/delete product or check the system statistics