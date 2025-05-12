#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

struct Item {
    string name;
    double price;
    int quantity;

    // Функция для отображения информации о товаре
    void display(int index) const {
        cout << index + 1 <<". " << name << " - Рублей " << price << " (Остаток: " << quantity << ")\n";
    }
};

class Cafeteria {
private:
    vector<Item> items; // Вектор для хранения товаров
    vector<Item> cart;  // Вектор для хранения товаров в корзине

public:
    Cafeteria() {
        loadItems(); // Загружаем товары из файла
    }

    void loadItems() {
        ifstream file("menu.txt");
        if (!file.is_open()) {
            cerr << "Невозможно открыть menu.txt. Пожалуйста создайте его и добавьте товар.\n";
            return;
        }

        string line;
        while (getline(file, line)) {
            istringstream iss(line);
            Item item;
            if (iss >> item.name >> item.price >> item.quantity) {
                items.push_back(item);
            }
        }
        file.close();
    }

    void saveItems() const {
        ofstream file("menu.txt");
        for (const auto& item : items) {
            file << item.name << " " << item.price << " " << item.quantity << "\n";
        }
        file.close();
    }

    void displayMenu() const {
        cout << "Меню:\n";
        for (size_t i = 0; i < items.size(); ++i) {
            items[i].display(i);
        }
    }

    void addToCart(int index, int quantity) {
        if (index < 0 || index >= items.size() || quantity > items[index].quantity) {
            cout << "Нету позиции или недостаточно товара.\n";
            return;
        }
        Item cartItem = items[index];
        cartItem.quantity = quantity;
        cart.push_back(cartItem);
        items[index].quantity -= quantity; // Уменьшаем количество товара в базе
    }

    void viewCart() const {
        cout << "Корзина:\n";
        double total = 0;
        for (const auto& item : cart) {
            item.display(0);
            total += item.price * item.quantity;
        }
        cout << "Сумма: " << total << "Рублей\n";
    }

    void confirmOrder() {
        saveItems(); // Сохраняем обновленные данные в файл
        cart.clear(); // Очищаем корзину после подтверждения заказа
        cout << "Заказ оформлен!\n";
    }

    void addItemToMenu() {
        Item newItem;
        cout << "Введите название товара: ";
        cin >> newItem.name;
        cout << "Стоимость товара: ";
        cin >> newItem.price;
        cout << "Количество товара: ";
        cin >> newItem.quantity;

        items.push_back(newItem);
        saveItems(); // Сохраняем изменения в файл
        cout << "Товар добавлен в меню.\n";
    }
};

int main() {
    Cafeteria cafeteria;
    int choice, quantity;

    while (true) {
        cout << "\n1. Посмотреть меню\n2. Добавить в корзину\n3. Посмотреть корзину\n4. Подтвердить заказ\n5. Добавить товар в меню\n6. Выйти\nВыбрать опцию: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cafeteria.displayMenu();
                break;
            case 2:
                int index;
                cout << "Введите номер товара и его количество: ";
                cin >> index >> quantity;
                cafeteria.addToCart(index - 1, quantity); // Индексы начинаются с 0
                break;
            case 3:
                cafeteria.viewCart();
                break;
            case 4:
                cafeteria.confirmOrder();
                break;
            case 5:
                cafeteria.addItemToMenu();
                break;
            case 6:
                return 0;
            default:
            cout << "Такой опции нет. Попробуйте еще раз!\n";
        }
    }

    return 0;
}