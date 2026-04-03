using luo.dangxiao.interfaces.ViewModels;
using System;
using System.Collections.Generic;
using System.Text;

namespace luo.dangxiao.interfaces.Views
{
    public interface IPageView
    {
        IPageViewModel ViewModel { get; }
    }
}
