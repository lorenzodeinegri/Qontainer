#ifndef SCULPTURE_H
#define SCULPTURE_H

#include "artwork.h"

class Sculpture : public ArtWork {
private:
    std::string shape;

    static float sculpture_restoration_overcharge;
    static float sculpture_loan_overcharge;

protected:
    virtual float calculateRestorationCost() const override;
    virtual float calculateLoanProceed() const override;

public:
    explicit Sculpture(unsigned int = 0,
                       float = 0.0f,
                       bool = false,
                       bool = false,
                       bool = true,
                       const std::string & = "Sconosciuta",
                       const std::string & = "Sconosciuto",
                       const std::string & = "Sconosciuta",
                       const std::string & = "Sconosciuto",
                       const std::string & = "Sconosciuto",
                       const std::string & = "Sconosciuto",
                       const std::string & = "Sconosciuto",
                       const std::string & = "Sconosciuto",
                       const date & = std::string("01/01/0100"),
                       const std::string & = ":/Photos/photoNotAvailable.jpeg",
                       unsigned int = 0,
                       unsigned int = 0,
                       float = 0.0f,
                       float = 0.0f);

    std::string getShape() const;

    void setShape(const std::string & = "Sconosciuta");

    virtual Sculpture * clone() const override;
    virtual std::string getType() const override;
    virtual std::string getInfo() const override;
    virtual float calculateValue() const override;
};

#endif // SCULPTURE_H
