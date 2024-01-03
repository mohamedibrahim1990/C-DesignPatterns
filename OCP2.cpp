//working code even with overloaded opeartor &&
#include <iostream>
#include <cstdint>
#include <string>
#include <vector>

enum class Color{Red , Green , Blue};
enum class Size{Small, Medium , Large};

struct Product{
    std::string name;
    Color color;
    Size size;
};

struct ProductFilter{
    typedef std::vector<Product*> Items;
public:
    Items by_color(Items items, Color color);
    Items by_size(Items items, Size size);
    Items by_color_size(Items items, Color color, Size size);
};

ProductFilter::Items ProductFilter::by_color(Items items, Color color){
    Items result;
    for(auto& i : items){
        if(i->color ==color){
            result.push_back(i);
            }
    }
    return result;
}

ProductFilter::Items ProductFilter::by_size(Items items, Size size){
    Items result;
    for(auto& i : items){
        if(i->size ==size){
            result.push_back(i);
            }
    }
    return result;
}

ProductFilter::Items ProductFilter::by_color_size(Items items, Color color, Size size){
     Items result;
    for(auto& i : items){
        if(i->size ==size && i->color==color){
            result.push_back(i);
            }
    }
    return result;
}

template<typename T>
struct AndSpecification;

template<typename T>
struct Specification{
    virtual bool is_satisfied(T* item)=0;
    virtual ~Specification()=default;
    AndSpecification<T> operator &&(Specification&& other){
        return AndSpecification<T>(*this, other);
    }
};
template <typename T> 
struct Filter{
    virtual std::vector<T*> filter(std::vector<T*> items, Specification<T>& spec)=0;
};

struct BetterFilter : Filter<Product>{
        std::vector<Product*> filter(std::vector<Product*> items, Specification<Product>& spec) override{
            std::vector<Product*> result;
            for(auto& p: items){
                if(spec.is_satisfied(p))
                    result.push_back(p);
            }
            return result;
        }
};
struct ColorSpecification:Specification<Product>{
    Color color;
    explicit ColorSpecification(const Color color) : color{color}{}
    bool is_satisfied(Product* item) override{
        return item->color==color;
    }
};

struct SizeSpecification:Specification<Product>{
    Size size;
    explicit SizeSpecification(const Size size) : size{size}{}
    bool is_satisfied(Product* item) override{
        return item->size==size;
    }
};

template<typename T>
struct AndSpecification: Specification<T>{
    Specification<T>&  first;
    Specification<T>& second;

    AndSpecification(Specification<T>& first, Specification<T>& second):first{first}, second{second}{}
    bool is_satisfied(T* item) override{
         return first.is_satisfied(item) && second.is_satisfied(item);       
    }
};


int main() { 
    Product apple{"Apple", Color::Green, Size::Small};
    Product tree{ "Tree", Color::Green, Size::Large };
    Product house{ "House", Color::Blue, Size::Large };

    /*std::vector<Product*> all{ &apple, &tree, &house };
    BetterFilter bf;
    ColorSpecification green(Color::Green);
    auto green_things = bf.filter(all, green);
    for (auto& x : green_things)
         std::cout << x->name << " is green" << std::endl;*/

    std::vector<Product*> all{ &apple, &tree, &house };
    BetterFilter bf;
    SizeSpecification large(Size::Large);
    ColorSpecification green(Color::Green);
    AndSpecification<Product> green_and_large{ large, green };
    auto big_green_things = bf.filter(all, green_and_large);
    for (auto& x : big_green_things)
        std::cout << x->name << " is large and green" << std::endl;

    auto green_and_big =
    ColorSpecification(Color::Green)
    && SizeSpecification(Size::Large);
    }
