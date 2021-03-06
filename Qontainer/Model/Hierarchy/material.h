#ifndef MATERIAL_H
#define MATERIAL_H

#include "date.h"

#include <string>
#include <sstream>
#include <iomanip>

class Material {
private:
    unsigned int sector;

    float base_value;

    bool proprietary;
    bool damaged;
    bool available;

    std::string author;
    std::string title;
    std::string realization_place;
    date realization_date;

    std::string photo;

    unsigned int restorations;
    unsigned int loans;

    float income;
    float expense;

    static float base_restoration_cost;
    static float base_loan_deposit;

protected:
    virtual float calculateRestorationCost() const;
    virtual float calculateLoanProceed() const;

public:
    explicit Material(unsigned int = 0,
                      float = 0.0f,
                      bool = false,
                      bool = false,
                      bool = true,
                      const std::string & = "Sconosciuto",
                      const std::string & = "Sconosciuto",
                      const std::string & = "Sconosciuto",
                      const date & = std::string("01/01/0100"),
                      const std::string & = ":/Photos/photoNotAvailable.jpeg",
                      unsigned int = 0,
                      unsigned int = 0,
                      float = 0.0f,
                      float = 0.0f);
    virtual ~Material() = default;

    std::string getAuthor() const;
    std::string getTitle() const;
    std::string getPlace() const;
    date getDate() const;
    std::string getPhoto() const;
    unsigned int getSector() const;
    bool isProprietary() const;
    bool isDamaged() const;
    bool isAvailable() const;
    unsigned int getRestorations() const;
    unsigned int getLoans() const;
    float getIncome() const;
    float getExpense() const;
    float getBaseValue() const;

    void setAuthor(const std::string & = "Sconosciuto");
    void setTitle(const std::string & = "Sconosciuto");
    void setPlace(const std::string & = "Sconosciuto");
    void setDate(const date & = std::string("01/01/0100"));
    void setPhoto(const std::string & = ":/Photos/photoNotAvailable.jpeg");
    void setSector(unsigned int = 0);
    void setProprietary(bool = true);
    void setDamaged(bool = true);
    void setAvailable(bool = true);
    void setRestorations(unsigned int = 0);
    void setLoans(unsigned int = 0);
    void setIncome(float = 0.0f);
    void setExpense(float = 0.0f);
    void setBaseValue(float = 0.0f);

    virtual Material * clone() const = 0;
    virtual std::string getType() const = 0;
    virtual std::string getMaterialType() const = 0;
    virtual std::string getInfo() const;
    virtual float calculateValue() const;

    bool restore();
    bool lend();
    float calculateProfit();

    static std::string float_to_string(float);
};

#endif // MATERIAL_H
