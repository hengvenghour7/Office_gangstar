class Drawing {
    protected:
    float Y{};
    
    public:
    Drawing();
    float getY();
    void setY(float Y);
    virtual void draw();
};