CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude -fPIC
LDFLAGS = -shared

# Цели сборки
all: liblogging.so test_app

# Сборка динамической библиотеки
liblogging.so: library/logging.cpp library/logging.hpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $<

# Сборка тестового приложения
test_app: app/app.cpp liblogging.so
	$(CXX) $(CXXFLAGS) -o $@ $< -L. -llogging -pthread

# Очистка
clean:
	rm -f liblogging.so test_app