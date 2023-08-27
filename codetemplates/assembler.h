
    /**
     * @brief Нет операции
     */
    void nop()
    {
        op1(OpCode::NOP);
    }
    /**
     * @brief Остановка машины, возврат, программа полностью выполнена
     */
    void halt()
    {
        op1(OpCode::HALT);
    }
    /**
     * @brief Загружает в стек 64-битное целое без знака
     */
    void ldc_uint64_data64(uint64_t data)
    {
        op2(OpCode::LDC_UINT64_DATA64, data);
    }
    /**
     * @brief Загружает в стек 64-битное целое со знаком
     */
    void ldc_int64_data64(int64_t data)
    {
        op2(OpCode::LDC_INT64_DATA64, data);
    }
    /**
     * @brief Загружает в стек адрес переменной
     */
    void ldloc_m(uint64_t data)
    {
        op2(OpCode::LDLOC_M, data);
    }
    /**
     * @brief Загружает в стек 64-битное вещественное число
     */
    void ldc_double_data64(double data)
    {
        op2(OpCode::LDC_DOUBLE_DATA64, data);
    }
    /**
     * @brief Вынимает из стека два числа и складывает их. Результат 
     * помещается обратно в стек.
     */
    void addst()
    {
        op1(OpCode::ADDST);
    }
    /**
     * @brief Вынимает из стека два числа и перемножает их. Результат 
     * помещается обратно в стек.
     */
    void mulst()
    {
        op1(OpCode::MULST);
    }
    /**
     * @brief Вынимает из стека два числа и делит нижний элемент на 
     * верхний. Результат помещается обратно в стек.
     */
    void divst()
    {
        op1(OpCode::DIVST);
    }
    /**
     * @brief Вынимает из стека два числа и вычитает нижний элемент 
     * из верхнего. Результат помещается обратно в стек.
     */
    void subst()
    {
        op1(OpCode::SUBST);
    }
    /**
     * @brief Вынимает из стека значение и размещает его по адресу 
     * M. M — указатель на Symbol.
     */
    void stloc_m(uint64_t data)
    {
        op2(OpCode::STLOC_M, data);
    }
    /**
     * @brief Вынимает из стека число и меняет его знак. Результат 
     * помещается обратно в стек.
     */
    void neg()
    {
        op1(OpCode::NEG);
    }
    /**
     * @brief Выполняет безусловный переход по указанному адресу.
     */
    void jmp_m(uint64_t data)
    {
        op2(OpCode::JMP_M, data);
    }
    /**
     * @brief Загружает в стек логическое значение. 1 — true, 0 — false.
     */
    void ldc_bool_data8(uint8_t data)
    {
        op2(OpCode::LDC_BOOL_DATA8, data);
    }
    /**
     * @brief Вынимает значение из стека и если значение равно 0, false, 
     * делает переход по адресу M.
     */
    void iffalse_m(uint64_t data)
    {
        op2(OpCode::IFFALSE_M, data);
    }
    /**
     * @brief Вынимает из стека два операнда, сравнивает их, если нижний 
     * элемент меньше верхнего, то результат true, иначе — false. Результат 
     * заносится в стек в виде константы true/false.
     */
    void stless()
    {
        op1(OpCode::STLESS);
    }
    /**
     * @brief Вынимает из стека два операнда, сравнивает их, если нижний 
     * элемент меньше или равен верхнему, то результат true, иначе 
     * — false. Результат заносится в стек в виде константы true/false.
     */
    void stless_eq()
    {
        op1(OpCode::STLESS_EQ);
    }
    /**
     * @brief Вынимает из стека два операнда, сравнивает их, если нижний 
     * элемент больше верхнего, то результат true, иначе — false. Результат 
     * заносится в стек в виде константы true/false.
     */
    void stgt()
    {
        op1(OpCode::STGT);
    }
    /**
     * @brief Вынимает из стека два операнда, сравнивает их, если нижний 
     * элемент больше или равен верхнему, то результат true, иначе 
     * — false. Результат заносится в стек в виде константы true/false.
     */
    void stgt_eq()
    {
        op1(OpCode::STGT_EQ);
    }
    /**
     * @brief Вынимает из стека два операнда, сравнивает их, если нижний 
     * элемент равен верхнему, то результат true, иначе — false. Результат 
     * заносится в стек в виде константы true/false.
     */
    void steq()
    {
        op1(OpCode::STEQ);
    }
    /**
     * @brief Загружает аргументы из стека в переменную arguments
     */
    void ldargs()
    {
        op1(OpCode::LDARGS);
    }
    /**
     * @brief Возврат из функции. Из стека вынимается предыдущее значение 
     * PC, и происходит переход к нему.
     */
    void ret()
    {
        op1(OpCode::RET);
    }
    /**
     * @brief Вызывает функцию, расположенную по указанному адресу.
     */
    void call(uint64_t data)
    {
        op2(OpCode::CALL, data);
    }
    /**
     * @brief Загружает в стек индекс строки. Строка устанавливается 
     * в таблицу строк.
     */
    void ldstring(uint64_t data)
    {
        op2(OpCode::LDSTRING, data);
    }
    /**
     * @brief Вызывает метод объекта автоматизации. Последним в стеке 
     * находится строка, загруженная с помощью LDSTRING, означающая 
     * имя метода.
     */
    void callm()
    {
        op1(OpCode::CALLM);
    }
    /**
     * @brief Создаёт массив. Аргумент — Symbol*. Если массива нет, 
     * он устанавливается в таблицу объектов.
     */
    void allocarray(uint64_t data)
    {
        op2(OpCode::ALLOCARRAY, data);
    }
    /**
     * @brief Извлекает из стека аргумент, увеличивает на 1 и снова 
     * помещает его в стек. Аргумент: число или переменная в стеке.
     */
    void inc()
    {
        op1(OpCode::INC);
    }
    /**
     * @brief Извлекает из стека аргумент, уменьшает на 1 и снова помещает 
     * его в стек. Аргумент: число или переменная в стеке.
     */
    void dec()
    {
        op1(OpCode::DEC);
    }
    /**
     * @brief Удаляет верхний элемент из стека, никуда его не присваивая.
     */
    void pop()
    {
        op1(OpCode::POP);
    }
    /**
     * @brief Сдвиг вправо со знаком (если спереди 1, то она остаётся). 
     * В стеке два операнда, №2 сверху. Операнд 1 сдвигается на величину 
     * операнда 2. Результат помещается в стек.
     */
    void rsh()
    {
        op1(OpCode::RSH);
    }
    /**
     * @brief Сдвиг вправо с заполнением нулями. В стеке два операнда, 
     * №2 сверху. Операнд 1 сдвигается на величину операнда 2. Результат 
     * помещается в стек.
     */
    void rshz()
    {
        op1(OpCode::RSHZ);
    }
    /**
     * @brief Сдвиг влево. В стеке два операнда, №2 сверху. Операнд 
     * 1 сдвигается на величину операнда 2. Результат помещается в 
     * стек.
     */
    void lsh()
    {
        op1(OpCode::LSH);
    }
    /**
     * @brief Побитовое И между двумя операндами в стеке. Результат 
     * помещается в стек.
     */
    void bit_and()
    {
        op1(OpCode::BIT_AND);
    }
    /**
     * @brief Побитовое ИЛИ между двумя операндами в стеке. Результат 
     * помещается в стек.
     */
    void bit_or()
    {
        op1(OpCode::BIT_OR);
    }
    /**
     * @brief Побитовое ИСКЛЮЧАЮЩЕЕ ИЛИ между двумя операндами в стеке. 
     * Результат помещается в стек.
     */
    void bit_xor()
    {
        op1(OpCode::BIT_XOR);
    }
    /**
     * @brief Логическое И между двумя операндами в стеке. Результат 
     * помещается в стек.
     */
    void log_and()
    {
        op1(OpCode::LOG_AND);
    }
    /**
     * @brief Логическое ИЛИ между двумя операндами в стеке. Результат 
     * помещается в стек.
     */
    void log_or()
    {
        op1(OpCode::LOG_OR);
    }
    /**
     * @brief Побитовое НЕ над элементом в стеке. Результат помещается 
     * в стек.
     */
    void bit_not()
    {
        op1(OpCode::BIT_NOT);
    }
    /**
     * @brief Побитовое НЕ над элементом в стеке. Результат помещается 
     * в стек.
     */
    void log_not()
    {
        op1(OpCode::LOG_NOT);
    }
    /**
     * @brief Остаток от деления между двумя операндами в стеке. Результат 
     * помещается в стек.
     */
    void modst()
    {
        op1(OpCode::MODST);
    }
    /**
     * @brief Сохраняет запись активации. Аргумент: 32бит старшие — 
     * смещение до начала таблицы переменных в функции, 32 бит младшие 
     * — следующая позиция после последней переменной в таблице переменных 
     * функции. Эта команда не работает вне функций (игнорируется при 
     * генерации кода).
     */
    void st_ar(uint64_t data)
    {
        op2(OpCode::ST_AR, data);
    }
    /**
     * @brief Загружает запись активации. Аргумент: 32бит старшие — 
     * смещение до начала таблицы переменных в функции, 32 бит младшие 
     * — следующая позиция после последней переменной в таблице переменных 
     * функции. Эта команда не работает вне функций (игнорируется при 
     * генерации кода).
     */
    void ld_ar(uint64_t data)
    {
        op2(OpCode::LD_AR, data);
    }
    /**
     * @brief Загружает в стек NULL-значение
     */
    void ld_null()
    {
        op1(OpCode::LD_NULL);
    }
    /**
     * @brief Вынимает из стека два операнда, сравнивает их, если нижний 
     * элемент не равен верхнему, то результат true, иначе — false. 
     * Результат заносится в стек в виде константы true/false.
     */
    void stnoteq()
    {
        op1(OpCode::STNOTEQ);
    }


