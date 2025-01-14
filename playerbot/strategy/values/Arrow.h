#pragma once

namespace ai
{
    class UnitPosition
    {
    public:
        UnitPosition(float x, float y) : x(x), y(y) {}
        UnitPosition(const UnitPosition& other) { x = other.x; y = other.y; }
        float x, y;
    };

    class FormationUnit
    {
    public:
        FormationUnit(uint32 groupIndex, bool master) : groupIndex(groupIndex), master(master), position(0, 0) {}
        FormationUnit(const FormationUnit& other) : position(other.position.x, other.position.y)
        {
            groupIndex = other.groupIndex;
            master = other.master;
        }

    public:
        uint32 GetGroupIdex() { return groupIndex; }
        void SetLocation(UnitPosition pos) { position = pos; }
        void SetLocation(float x, float y) { position.x = x; position.y = y; }
        float GetX() { return position.x; }
        float GetY() { return position.y; }

    private:
        uint32 groupIndex;
        bool master;
        UnitPosition position;
    };

    class UnitPlacer
    {
    public:
        UnitPlacer(float range) : range(range) {}

    public:
        virtual UnitPosition Place(FormationUnit *unit, uint32 index, uint32 count) = 0;
    protected:
        float range = 0;
    };

    class FormationSlot
    {
    public:
        FormationSlot() {}
        virtual ~FormationSlot();

    public:
        void AddLast(FormationUnit* unit) { units.push_back(unit); }
        void InsertAtCenter(FormationUnit* unit) { units.insert(units.begin() + (units.size() + 1) / 2, unit); }
        void PlaceUnits(UnitPlacer* placer);
        void Move(float dx, float dy);
        int Size() { return units.size(); }

    private:
        WorldLocation center;
        std::vector<FormationUnit*> units;
    };


    class MultiLineUnitPlacer : public UnitPlacer
    {
    public:
        MultiLineUnitPlacer(float orientation, float range) : UnitPlacer(range), orientation(orientation) {}

    public:
        virtual UnitPosition Place(FormationUnit *unit, uint32 index, uint32 count);

    private:
        float orientation;
    };

    class SingleLineUnitPlacer
    {
    public:
        SingleLineUnitPlacer(float orientation, float range) : orientation(orientation), range(range) {}

    public:
        virtual UnitPosition Place(FormationUnit *unit, uint32 index, uint32 count);

    private:
        float orientation, range;
    };

    class ArrowFormation : public MoveAheadFormation
    {
    public:
        ArrowFormation(PlayerbotAI* ai) : MoveAheadFormation(ai, "arrow"), built(false), masterUnit(NULL), botUnit(NULL) {}

    public:
        virtual WorldLocation GetLocationInternal();

    private:
        void Build();
        void FillSlotsExceptMaster();
        void AddMasterToSlot();
        FormationSlot* FindSlot(Player* member);

    private:
        FormationSlot tanks, melee, ranged, healers;
        FormationUnit *masterUnit, *botUnit;
        bool built;
    };

}

